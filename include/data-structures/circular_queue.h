#pragma once

#include "utility/data_types.h"

#define MAX_QUEUE_SIZE 256


struct NextValue {
    bool valid;
    byte value;
    int index;
};


class CircularQueue {
    public:
        CircularQueue();
        ~CircularQueue() {};

        /**
         * @brief Inserts a byte into the circular queue at nextInsertIndex (m_dataTail + 1)
         * @param val Value to insert
         */
        void insert(byte val);

        /**
         * @brief Insert a sequence of characters into the buffer
         * @param buffer 
         * @param bufferLength 
         */
        void insert(const char* buffer, size_t bufferLength);

        /**
         * @brief Get the index of the end of the m_data. The length of the queue is tailPos + 1
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

        /**
         * @brief Get the next unread value from the buffer.
         * @return NextValue 
         */
        NextValue getNextValue();

        /**
         * @brief Set the Value Index object
         * @param i 
         */
        void setSearchIndex(int i);

        /**
         * @brief Set the Insert Index variable
         * @param i 
         */
        void setInsertIndex(int i);

        /**
         * @brief Copies the bytes out of m_data into buffer between the specified indices
         * @param buffer Assumes the buffer has enough space allocated for it to not seg fault
         * @param minIndex 
         * @param maxIndex 
         */
        void copyData(byte* buffer, int minIndex, int maxIndex);


        /**
         * @brief Resets the queue to initialised values. i.e, 0s in m_data and hasDataBeenRead
         * dataTail is set to the first element. Next insert and search index are set to the first element
         */
        void reset();

        /**
         * @brief Looks for a character in the queue.
         * @param ch - A valid character from the ascii table. 
         * @return int - The index position of the element. -1 Indicates it is not in the list
         */
        int searchForCharacter(char ch);

        /**
         * @brief Replaces (or inserts) "value" at "index"
         * @param value 
         * @param index 
         */
        void replace(byte value, int index);

    protected:
        byte m_data[MAX_QUEUE_SIZE];
        bool m_hasDataBeenRead[MAX_QUEUE_SIZE];
        int m_dataTail;
        autoIncrementingResetInt m_nextInsertPos;
        autoIncrementingResetInt m_searchIndex;

};