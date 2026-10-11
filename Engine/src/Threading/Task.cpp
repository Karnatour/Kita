#include "Task.h"

namespace Kita {
    Task::Task(const Priority priority, const size_t taskID, std::move_only_function<void()> workFunction) :
        m_priority(priority), m_taskID(taskID), m_workFunction(std::move(workFunction)) {
    }

    Task::Priority Task::getPriority() const {
        return m_priority;
    }

    size_t Task::getTaskID() const {
        return m_taskID;
    }

    std::move_only_function<void()>& Task::getWorkFunction() {
        return m_workFunction;
    }
} // Kita
