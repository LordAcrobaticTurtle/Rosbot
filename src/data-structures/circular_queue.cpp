#include "data-structures/circular_queue.h"
#include <cstring>

CircularQueue::CircularQueue() {
    dataTail = 0;
    nextInsertPos = 0;
    memset(m_data, 0, sizeof(byte)*MAX_QUEUE_SIZE);
}

int CircularQueue::getTailPos() {
    return dataTail;
}

void CircularQueue::insert(byte val) {
    if (nextInsertPos == MAX_QUEUE_SIZE) {
        nextInsertPos = 0;
    }

    m_data[nextInsertPos++] = val;
    dataTail = nextInsertPos - 1;
}

byte &CircularQueue::operator[](int i) {
    return m_data[i % MAX_QUEUE_SIZE];
}