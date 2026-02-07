/**
 * test_thread_pool.cpp - Tests for ThreadPool
 */

#include "strata/util/thread_pool.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

using namespace strata::util;

void test_basic_submit() {
    ThreadPool pool(4);

    auto future = pool.submit([]() { return 42; });
    int result = future.get();

    assert(result == 42);
    std::cout << "✓ test_basic_submit passed\n";
}

void test_multiple_tasks() {
    ThreadPool pool(4);

    std::vector<std::future<int>> futures;
    for (int i = 0; i < 100; ++i) {
        futures.push_back(pool.submit([i]() { return i * 2; }));
    }

    for (int i = 0; i < 100; ++i) {
        int result = futures[i].get();
        assert(result == i * 2);
    }

    std::cout << "✓ test_multiple_tasks passed\n";
}

void test_concurrent_execution() {
    ThreadPool pool(4);
    std::atomic<int> counter{0};

    std::vector<std::future<void>> futures;
    for (int i = 0; i < 100; ++i) {
        futures.push_back(pool.submit([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    for (auto& f : futures) {
        f.get();
    }

    assert(counter.load() == 100);
    std::cout << "✓ test_concurrent_execution passed\n";
}

void test_bulk_submit() {
    ThreadPool pool(4);

    std::vector<std::function<int()>> tasks;
    for (int i = 0; i < 50; ++i) {
        tasks.push_back([i]() { return i * 3; });
    }

    std::vector<std::future<int>> futures;
    pool.submit_bulk(tasks, futures);

    assert(futures.size() == 50);

    for (int i = 0; i < 50; ++i) {
        int result = futures[i].get();
        assert(result == i * 3);
    }

    std::cout << "✓ test_bulk_submit passed\n";
}

void test_thread_count() {
    ThreadPool pool1(4);
    assert(pool1.thread_count() == 4);

    ThreadPool pool2(8);
    assert(pool2.thread_count() == 8);

    ThreadPool pool3(1);
    assert(pool3.thread_count() == 1);

    std::cout << "✓ test_thread_count passed\n";
}

void test_is_running() {
    ThreadPool pool(2);
    assert(pool.is_running());

    pool.shutdown();
    assert(!pool.is_running());

    std::cout << "✓ test_is_running passed\n";
}

void test_shutdown_completes_tasks() {
    ThreadPool pool(2);
    std::atomic<int> completed{0};

    // Submit some tasks
    for (int i = 0; i < 10; ++i) {
        pool.submit([&completed]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            completed.fetch_add(1, std::memory_order_relaxed);
        });
    }

    // Shutdown waits for tasks
    pool.shutdown();

    assert(completed.load() == 10);
    std::cout << "✓ test_shutdown_completes_tasks passed\n";
}

void test_exception_in_task() {
    ThreadPool pool(2);

    auto future = pool.submit([]() -> int {
        throw std::runtime_error("test exception");
        return 0;
    });

    bool caught = false;
    try {
        future.get();
    } catch (const std::runtime_error& e) {
        caught = true;
        assert(std::string(e.what()) == "test exception");
    }

    assert(caught);
    std::cout << "✓ test_exception_in_task passed\n";
}

void test_submit_with_arguments() {
    ThreadPool pool(2);

    auto future = pool.submit([](int a, int b) { return a + b; }, 10, 20);
    int result = future.get();

    assert(result == 30);
    std::cout << "✓ test_submit_with_arguments passed\n";
}

void test_void_tasks() {
    ThreadPool pool(2);
    std::atomic<bool> executed{false};

    auto future = pool.submit([&executed]() {
        executed.store(true, std::memory_order_relaxed);
    });

    future.get();
    assert(executed.load());

    std::cout << "✓ test_void_tasks passed\n";
}

void test_default_thread_count() {
    ThreadPool pool;  // Default constructor
    assert(pool.thread_count() > 0);

    std::cout << "✓ test_default_thread_count passed (threads=" << pool.thread_count() << ")\n";
}

void test_single_thread_pool() {
    ThreadPool pool(1);

    std::vector<int> results;
    std::mutex mtx;

    std::vector<std::future<void>> futures;
    for (int i = 0; i < 10; ++i) {
        futures.push_back(pool.submit([&results, &mtx, i]() {
            std::lock_guard<std::mutex> lock(mtx);
            results.push_back(i);
        }));
    }

    for (auto& f : futures) {
        f.get();
    }

    assert(results.size() == 10);

    // With single thread, results should be in order
    for (int i = 0; i < 10; ++i) {
        assert(results[i] == i);
    }

    std::cout << "✓ test_single_thread_pool passed\n";
}

void test_many_threads() {
    // Test with more threads than typical
    ThreadPool pool(16);
    std::atomic<int> counter{0};

    std::vector<std::future<void>> futures;
    for (int i = 0; i < 1000; ++i) {
        futures.push_back(pool.submit([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        }));
    }

    for (auto& f : futures) {
        f.get();
    }

    assert(counter.load() == 1000);
    std::cout << "✓ test_many_threads passed\n";
}

int main() {
    std::cout << "Running ThreadPool tests...\n\n";

    test_basic_submit();
    test_multiple_tasks();
    test_concurrent_execution();
    test_bulk_submit();
    test_thread_count();
    test_is_running();
    test_shutdown_completes_tasks();
    test_exception_in_task();
    test_submit_with_arguments();
    test_void_tasks();
    test_default_thread_count();
    test_single_thread_pool();
    test_many_threads();

    std::cout << "\nAll ThreadPool tests passed!\n";
    return 0;
}
