#include <data-structures/circular_queue.h>
#include <utility/math.h>
#include <stdlib.h>
#include <cstring>

CircularQueue::CircularQueue() {
    m_dataTail = 0;
    m_nextInsertPos = 0;
    m_currValueIndex = 0;
    memset(m_data, 0, sizeof(byte)*MAX_QUEUE_SIZE);
    memset(m_hasDataBeenRead, 0, sizeof(bool)*MAX_QUEUE_SIZE);
}

int CircularQueue::getTailPos() {
    return m_dataTail;
}

void CircularQueue::insert(byte val) {
    if (m_nextInsertPos == MAX_QUEUE_SIZE) {
        m_nextInsertPos = 0;
    }
    m_hasDataBeenRead[m_nextInsertPos] = false;
    m_data[m_nextInsertPos++] = val;
    m_dataTail = m_nextInsertPos - 1;
}

byte &CircularQueue::operator[](int i) {
    return m_data[abs(i) % MAX_QUEUE_SIZE];
}

NextValue CircularQueue::getNextValue() {
    NextValue ret;
    
    if (m_hasDataBeenRead[m_currValueIndex]) {
        ret.valid = false;
        ret.value = 0;
        ret.index = m_currValueIndex++;
        return ret;
    }

    m_hasDataBeenRead[m_currValueIndex] = true;
    ret.index = m_currValueIndex;
    ret.valid = true;
    ret.value = m_data[m_currValueIndex++];
    return ret;
}

void CircularQueue::setCurrValueIndex(int i) {
    int value = clamp(i, 0, MAX_QUEUE_SIZE);
    m_currValueIndex = value;
}

void CircularQueue::setInsertIndex(int i) {
    int value = clamp(i, 0, MAX_QUEUE_SIZE);
    m_nextInsertPos = value;
}