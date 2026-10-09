#pragma once
#include <future>
#include <mutex>
#include <thread>
#include <vector>

#include "Task.h"
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API ThreadPool {
    public:
        ThreadPool();
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool& operator=(const ThreadPool&) = delete;
        ~ThreadPool();

        template <typename Function, typename... Args>
        auto submit(Task::Priority priority, Function&& function, Args&&... arg) -> std::future<std::invoke_result_t<Function, Args...>> {
            auto task = std::packaged_task<std::invoke_result_t<Function, Args...>()>(
                std::bind_front(
                    std::forward<Function>(function),
                    std::forward<Args>(arg)...)
            );

            std::future<std::invoke_result_t<Function, Args...>> result = task.get_future();
            {
                std::lock_guard lock(m_tasksVectorMutex);
                if (m_stopRequested) {
                    throw std::runtime_error("[ThreadPool] Tried to submit task after stop was requested");
                }
                m_tasks.emplace_back(priority, m_nextTaskID++);
                reorderTasks();
            }

            m_condition.notify_one();
            return result;
        };

    private:
        void workerLoop(const std::stop_token& stopToken);
        void reorderTasks();

        std::mutex m_tasksVectorMutex;
        std::condition_variable_any m_condition;
        std::vector<Task> m_tasks;
        size_t m_nextTaskID = 0;
        bool m_stopRequested = false;
        std::vector<std::jthread> m_workers;
    };
} // Kita
