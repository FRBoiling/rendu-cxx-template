/*
* Created by boil on 25-2-28.
*/

#ifndef THREADPOOL_HPP
#define THREADPOOL_HPP

#include <functional>
#include <queue>
#include <thread>

namespace async {

    // 自定义线程池实现
    class ThreadPool {
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable condition;
        std::atomic_bool stop{false};

    public:
        explicit ThreadPool(size_t threads = std::thread::hardware_concurrency()) {
            for (size_t i = 0; i < threads; ++i) {
                workers.emplace_back([this] {
                    while (true) {
                        std::function<void()> task;
                        {
                            std::unique_lock lock(queue_mutex);
                            condition.wait(lock, [this] {
                                return stop || !tasks.empty();
                            });
                            if (stop && tasks.empty()) return;
                            task = std::move(tasks.front());
                            tasks.pop();
                        }
                        task();
                    }
                });
            }
        }

        template<class F>
        void enqueue(F&& task) {
            {
                std::unique_lock lock(queue_mutex);
                tasks.emplace(std::forward<F>(task));
            }
            condition.notify_one();
        }

        ~ThreadPool() {
            stop = true;
            condition.notify_all();
            for (auto& worker : workers) {
                if (worker.joinable()) worker.join();
            }
        }
    };
}




#endif //THREADPOOL_HPP
