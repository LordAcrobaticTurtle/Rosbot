#include <data-structures/circular_queue.h>
#include <utility/math.h>

#include <cstring>

CircularQueue::CircularQueue() {
    m_dataTail = 0;
    m_nextInsertPos = 0;
    m_currValueIndex = 0;
    memset(m_data, 0, sizeof(byte)*MAX_QUEUE_SIZE);
}

int CircularQueue::getTailPos() {
    return m_dataTail;
}

void CircularQueue::insert(byte val) {
    if (m_nextInsertPos == MAX_QUEUE_SIZE) {
        m_nextInsertPos = 0;
    }

    m_data[m_nextInsertPos++] = val;
    m_dataTail = m_nextInsertPos - 1;
}

byte &CircularQueue::operator[](int i) {
    return m_data[abs(i) % MAX_QUEUE_SIZE];
}

byte CircularQueue::getNextValue() {
    return m_data[m_currValueIndex++];
}

void CircularQueue::setCurrValueIndex(int i) {
    int value = clamp(i, 0, MAX_QUEUE_SIZE);
    m_currValueIndex = value;
}