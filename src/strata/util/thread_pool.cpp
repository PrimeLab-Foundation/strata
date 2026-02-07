#include "strata/util/thread_pool.hpp"

#include <algorithm>

namespace strata {
namespace util {

ThreadPool::ThreadPool(size_t num_threads) {
    // Default to hardware concurrency if not specified
    if (num_threads == 0) {
        num_threads = std::thread::hardware_concurrency();
        // Fallback to 4 threads if hardware_concurrency returns 0
        if (num_threads == 0) {
            num_threads = 4;
        }
    }

    // Limit maximum threads to avoid excessive context switching
    num_threads = std::min(num_threads, static_cast<size_t>(256));

    workers_.reserve(num_threads);
    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back(&ThreadPool::worker_thread, this);
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::shutdown() {
    // Only shutdown once
    bool expected = false;
    if (!shutdown_requested_.compare_exchange_strong(expected, true)) {
        // Already shutting down, wait for workers
        for (auto& worker : workers_) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        return;
    }

    {
        std::lock_guard<std::mutex> lock(mutex_);
        stop_.store(true, std::memory_order_release);
    }

    cv_.notify_all();

    for (auto& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::worker_thread() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex_);

            // Wait for task or stop signal
            cv_.wait(lock, [this] {
                return stop_.load(std::memory_order_acquire) || !tasks_.empty();
            });

            // Exit if stopped and no more tasks
            if (stop_.load(std::memory_order_acquire) && tasks_.empty()) {
                return;
            }

            // Get task from queue
            if (!tasks_.empty()) {
                task = std::move(tasks_.front());
                tasks_.pop();
            }
        }

        // Execute task outside lock
        if (task) {
            task();
        }
    }
}

}  // namespace util
}  // namespace strata
