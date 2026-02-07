#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <vector>

namespace strata {
namespace util {

/**
 * Thread pool for parallel task execution.
 *
 * Provides a simple, efficient thread pool with minimal-contention task queue.
 * Designed for batch processing workloads like parallel NDJSON parsing.
 *
 * Features:
 * - Configurable thread count (defaults to hardware_concurrency)
 * - Future-based result collection
 * - Graceful shutdown with task completion
 * - Thread-safe task submission
 */
class ThreadPool {
  public:
    /**
     * Create thread pool with specified number of threads.
     *
     * @param num_threads Number of worker threads (0 = hardware_concurrency)
     */
    explicit ThreadPool(size_t num_threads = 0);

    /**
     * Destructor - waits for all tasks to complete.
     */
    ~ThreadPool();

    // Non-copyable, non-movable
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    /**
     * Submit a task for execution.
     *
     * @param f Callable to execute
     * @param args Arguments to pass to f
     * @return Future containing the result
     */
    template <typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>>;

    /**
     * Submit multiple tasks and collect futures.
     *
     * More efficient than individual submit() calls for bulk work.
     *
     * @tparam F Callable type
     * @param tasks Vector of tasks to submit
     * @param futures Output vector for futures (will be resized)
     */
    template <typename F>
    void submit_bulk(const std::vector<F>& tasks,
                     std::vector<std::future<std::invoke_result_t<F>>>& futures);

    /**
     * Get the number of worker threads.
     */
    size_t thread_count() const noexcept { return workers_.size(); }

    /**
     * Check if the pool is running.
     */
    bool is_running() const noexcept { return !stop_.load(std::memory_order_relaxed); }

    /**
     * Shutdown the pool and wait for completion.
     *
     * After shutdown, no new tasks can be submitted.
     * Pending tasks will be completed before workers exit.
     */
    void shutdown();

  private:
    void worker_thread();

    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stop_{false};
    std::atomic<bool> shutdown_requested_{false};
};

// ============================================================================
// Template Implementations
// ============================================================================

template <typename F, typename... Args>
auto ThreadPool::submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
    using ReturnType = std::invoke_result_t<F, Args...>;

    if (stop_.load(std::memory_order_relaxed)) {
        throw std::runtime_error("Cannot submit task to stopped thread pool");
    }

    // Create packaged task with bound arguments
    auto task = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...));

    std::future<ReturnType> result = task->get_future();

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stop_.load(std::memory_order_relaxed)) {
            throw std::runtime_error("Cannot submit task to stopped thread pool");
        }
        tasks_.emplace([task]() { (*task)(); });
    }

    cv_.notify_one();
    return result;
}

template <typename F>
void ThreadPool::submit_bulk(const std::vector<F>& tasks,
                             std::vector<std::future<std::invoke_result_t<F>>>& futures) {
    using ReturnType = std::invoke_result_t<F>;

    if (stop_.load(std::memory_order_relaxed)) {
        throw std::runtime_error("Cannot submit tasks to stopped thread pool");
    }

    futures.clear();
    futures.reserve(tasks.size());

    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stop_.load(std::memory_order_relaxed)) {
            throw std::runtime_error("Cannot submit tasks to stopped thread pool");
        }

        for (const auto& task_func : tasks) {
            auto task = std::make_shared<std::packaged_task<ReturnType()>>(task_func);
            futures.push_back(task->get_future());
            tasks_.emplace([task]() { (*task)(); });
        }
    }

    // Wake up all workers for bulk submission
    cv_.notify_all();
}

}  // namespace util
}  // namespace strata
