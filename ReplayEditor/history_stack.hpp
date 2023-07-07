#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <array>
#include <memory>

template <typename T, size_t MAX_HISTORY>
class HistoryStack
{
   private:
    std::array<std::unique_ptr<T>, MAX_HISTORY> m_arr;
    size_t m_index = 0;
    size_t m_undoable_count = 0;
    size_t m_redoable_count = 0;

   public:
    const T *current() const
    {
        return m_arr[(m_index - 1) % MAX_HISTORY].get();
    }

    T *current()
    {
        return m_arr[(m_index - 1) % MAX_HISTORY].get();
    }

    void push_mutation(std::unique_ptr<T> ptr)
    {
        m_arr[m_index] = std::move(ptr);
        m_index = (m_index + 1) % MAX_HISTORY;
        if (m_undoable_count < MAX_HISTORY) ++m_undoable_count;
        m_redoable_count = 0;
    }

    bool undo_mutation()
    {
        // Do not undo the very last thing, otherwise the current ptr will be null.
        if (m_undoable_count > 1) {
            --m_undoable_count;
            ++m_redoable_count;
            m_index = (m_index - 1) % MAX_HISTORY;
            return true;
        }
        return false;
    }

    bool redo_mutation()
    {
        if (m_redoable_count > 0) {
            m_index = (m_index + 1) % MAX_HISTORY;
            --m_redoable_count;
            ++m_undoable_count;
            return true;
        }
        return false;
    }
};