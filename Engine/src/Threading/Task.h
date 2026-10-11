#pragma once
#include <functional>
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
        Task(Priority priority, size_t taskID, std::move_only_function<void()> workFunction);

        Priority getPriority() const;
        size_t getTaskID() const;
        std::move_only_function<void()>& getWorkFunction();

    private:
        Priority m_priority = Priority::NORMAL;
        size_t m_taskID = 0;
        std::move_only_function<void()> m_workFunction;
    };
} // Kita
