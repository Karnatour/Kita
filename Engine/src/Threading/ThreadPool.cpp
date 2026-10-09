#include "ThreadPool.h"

namespace Kita {
    ThreadPool::ThreadPool() {
        int threadCount = std::thread::hardware_concurrency() - 1;
        if (threadCount <= 0) {
            threadCount = 1;
        }

        m_workers.reserve(threadCount);
        for (int i = 0; i < threadCount; ++i) {
            m_workers.emplace_back([this](const std::stop_token& stopToken) {
                workerLoop(stopToken);
            });
        }
        KITA_ENGINE_DEBUG("[ThreadPool] Created {} worker threads", threadCount);
    }

    ThreadPool::~ThreadPool() {
        // Extra {} so lock is instantly released
        {
            std::lock_guard lock(m_tasksVectorMutex);
            m_stopRequested = true;
        }
        for (auto& worker : m_workers) {
            worker.request_stop();
        }
    }

    void ThreadPool::reorderTasks() {
        std::ranges::push_heap(m_tasks, [](const Task& a, const Task& b) {
            if (a.getPriority() == b.getPriority()) {
                return a.getTaskID() > b.getTaskID();
            }
            return a.getPriority() < b.getPriority();
        });
    }

    void ThreadPool::workerLoop(const std::stop_token& stopToken) {
        while (true) {
            Task task;
            {
                std::unique_lock lock(m_tasksVectorMutex);
                m_condition.wait(lock, stopToken, [this] {
                    return !m_tasks.empty();
                });

                if (m_tasks.empty()) {
                    // If this is reached, it means stop has been requested so we exit infinite loop
                    return;
                }

                task = std::move(m_tasks.back());
                m_tasks.pop_back();
            }
            std::move_only_function<void()>& workFunction = task.getWorkFunction();
            //Execute the function
            workFunction();
        }
    }
} // Kita
