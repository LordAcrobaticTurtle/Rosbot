#pragma once

#include "utility/data_types.h"

#define MAX_QUEUE_SIZE 256

class CircularQueue {
    public:
        CircularQueue();
        ~CircularQueue() {};

        /**
         * @brief Inserts a byte into the circular queue at nextInsertIndex (dataTail + 1)
         * @param val Value to insert
         */
        void insert(byte val);

        /**
         * @brief Get the index of the end of the m_data
         * @return int Index of end of m_data.
         */
        int getTailPos();

        /**
         * @brief Array like accessing of m_data underneath. Takes modulo of input to stay within
         * array bounds. READ ONLY
         * @param i 
         * @return byte& 
         */
        byte &operator[](int i);

    protected:
        byte m_data[MAX_QUEUE_SIZE];
        int dataTail;
        int nextInsertPos;

};