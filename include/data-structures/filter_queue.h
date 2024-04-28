#pragma once
/**
 * @file filter_queue.h
 * @author Sam Hall
 * @brief Breaks my "No heap" rule. However isn't, core functionality
 * @version 0.1
 * @date 2024-04-25
 */

#include <memory>

template<typename T>
class FilterQueue  {
    public:
        FilterQueue (int numElements);
        ~FilterQueue ();

        T average ();

    private:
        std::unique_ptr<T[]> m_queue;
};