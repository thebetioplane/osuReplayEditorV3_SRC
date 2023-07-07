#pragma once
// clang-format off
#include "stdafx.h"
// clang-format on

#include <array>
#include <memory>

template <typename T, int MAX_HISTORY>
class HistoryStack
{
   public:
    const T *current() const
    {
        return m_arr[index_minus_one()].get();
    }

    T *current()
    {
        return m_arr[index_minus_one()].get();
    }

    void push_mutation(std::unique_ptr<T> ptr)
    {
        m_arr[m_index] = std::move(ptr);
        m_index = index_plus_one();
        if (m_undoable_count < MAX_HISTORY) ++m_undoable_count;
        m_redoable_count = 0;
    }

    bool undo_mutation()
    {
        // Do not undo the very last thing, otherwise the current ptr will be null.
        if (m_undoable_count > 1) {
            --m_undoable_count;
            ++m_redoable_count;
            m_index = index_minus_one();
            return true;
        }
        return false;
    }

    bool redo_mutation()
    {
        if (m_redoable_count > 0) {
            m_index = index_plus_one();
            --m_redoable_count;
            ++m_undoable_count;
            return true;
        }
        return false;
    }

   private:
    int index_plus_one() const
    {
        return (m_index + 1) % MAX_HISTORY;
    }

    int index_minus_one() const
    {
        if (m_index <= 0) {
            return MAX_HISTORY - 1;
        } else {
            return m_index - 1;
        }
    }

    std::array<std::unique_ptr<T>, MAX_HISTORY> m_arr;
    int m_index = 0;
    int m_undoable_count = 0;
    int m_redoable_count = 0;
};