#include <data-structures/circular_queue.h>
#include <utility/math.h>
#include <stdlib.h>
#include <cstring>

CircularQueue::CircularQueue() : 
    m_nextInsertPos(0, MAX_QUEUE_SIZE), m_searchIndex(0, MAX_QUEUE_SIZE) 
{
    m_dataTail = 0;
    memset(m_data, 0, sizeof(byte)*MAX_QUEUE_SIZE);
    memset(m_hasDataBeenRead, 0, sizeof(bool)*MAX_QUEUE_SIZE);
}

int CircularQueue::getTailPos() {
    return m_dataTail;
}

void CircularQueue::insert(byte val) {
    m_hasDataBeenRead[m_nextInsertPos.getCount()] = false;
    m_data[m_nextInsertPos.getCount()] = val;
    m_dataTail = m_nextInsertPos.getCount();
    m_nextInsertPos++;
}

void CircularQueue::insert(const char* buffer, size_t bufferLength) {
    for (size_t i = 0; i < bufferLength; i++) {
        insert(buffer[i]);
    }
}

byte &CircularQueue::operator[](int i) {
    return m_data[abs(i) % MAX_QUEUE_SIZE];
}

NextValue CircularQueue::getNextValue() {
    NextValue ret;
    
    if (m_hasDataBeenRead[m_searchIndex.getCount()]) {
        ret.valid = false;
        ret.value = 0;
        ret.index = m_searchIndex.getCount();
        m_searchIndex++;
        return ret;
    }

    m_hasDataBeenRead[m_searchIndex.getCount()] = true;
    ret.index = m_searchIndex.getCount();
    ret.valid = true;
    ret.value = m_data[m_searchIndex.getCount()];
    m_searchIndex++;
    return ret;
}

void CircularQueue::setSearchIndex(int i) {
    m_searchIndex.setCount(i);
}

void CircularQueue::setInsertIndex(int i) {
    m_nextInsertPos.setCount(i);
}

void CircularQueue::copyData(byte* buffer, int minIndex, int maxIndex) {
    int clampedMinIndex = clamp(minIndex, 0, MAX_QUEUE_SIZE - 1);
    int clampedMaxIndex = clamp(maxIndex, 0, MAX_QUEUE_SIZE - 1);

    if (clampedMaxIndex - clampedMinIndex == 0) { return; }

    // How many bytes between the indices?
    int byteDifference = (clampedMaxIndex - clampedMinIndex) *sizeof(byte) + sizeof(byte);
    memcpy(buffer, &m_data[minIndex], byteDifference);
}


void CircularQueue::reset() {
    setInsertIndex(0);
    setSearchIndex(0);
    memset(m_data, 0, sizeof(byte)*MAX_QUEUE_SIZE);
    memset(m_hasDataBeenRead, 0, sizeof(bool)*MAX_QUEUE_SIZE);
    m_dataTail = 0;
}
