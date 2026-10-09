#pragma once
#include <compare>
#include "ThreadPool.h"
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Task {
    public:
        enum class Priority {
            LOW = 0,
            NORMAL = 1,
            HIGH = 2
        };

        Task() = default;
        Task(Priority priority, size_t taskID);

        Priority getPriority() const;
        size_t getTaskID() const;
        std::move_only_function<void()>& getWorkFunction();
    private:
        Priority m_priority;
        size_t m_taskID;
        std::move_only_function<void()> m_workFunction;
    };
} // Kita
