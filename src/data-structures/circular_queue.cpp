#include <data-structures/circular_queue.h>
#include <utility/math.h>
#include <stdlib.h>
#include <cstring>

template class CircularQueue<char>;
template class CircularQueue<float>;

template <typename T>
CircularQueue<T>::CircularQueue () : 
    m_nextInsertPos(0, MAX_QUEUE_SIZE), m_searchIndex(0, MAX_QUEUE_SIZE) 
{
    m_dataTail = 0;
    memset(m_data, 0, sizeof(T)*MAX_QUEUE_SIZE);
    memset(m_hasDataBeenRead, 0, sizeof(bool)*MAX_QUEUE_SIZE);
}

template <typename T>
int CircularQueue<T>::getTailPos() {
    return m_dataTail;
}

template <typename T>
void CircularQueue<T>::insert(T val) {
    m_hasDataBeenRead[m_nextInsertPos.getCount()] = false;
    m_data[m_nextInsertPos.getCount()] = val;
    m_dataTail = m_nextInsertPos.getCount();
    m_nextInsertPos++;
}

template <typename T>
void CircularQueue<T>::insert(const T* buffer, size_t bufferLength) {
    for (size_t i = 0; i < bufferLength; i++) {
        insert(buffer[i]);
    }
}

template <typename T>
T &CircularQueue<T>::operator[](int i) {
    return m_data[abs(i) % MAX_QUEUE_SIZE];
}

template <typename T>
NextValue<T> CircularQueue<T>::getNextValue() {
    NextValue<T> ret;
    
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

template <typename T>
void CircularQueue<T>::setSearchIndex(int i) {
    m_searchIndex.setCount(i);
}

template <typename T>
void CircularQueue<T>::setInsertIndex(int i) {
    m_nextInsertPos.setCount(i);
}

template <typename T>
void CircularQueue<T>::copyData(T* buffer, int minIndex, int maxIndex) {
    int clampedMinIndex = clamp(minIndex, 0, MAX_QUEUE_SIZE - 1);
    int clampedMaxIndex = clamp(maxIndex, 0, MAX_QUEUE_SIZE - 1);

    if (clampedMaxIndex - clampedMinIndex == 0) { return; }

    // How many bytes between the indices?
    int byteDifference = (clampedMaxIndex - clampedMinIndex) *sizeof(byte) + sizeof(byte);
    memcpy(buffer, &m_data[minIndex], byteDifference);
}

template <typename T>
void CircularQueue<T>::reset() {
    setInsertIndex(0);
    setSearchIndex(0);
    memset(m_data, 0, sizeof(byte)*MAX_QUEUE_SIZE);
    memset(m_hasDataBeenRead, 0, sizeof(bool)*MAX_QUEUE_SIZE);
    m_dataTail = 0;
}

template <typename T>
int CircularQueue<T>::searchForCharacter(T ch) {
    for (int i=0; i<getTailPos() + 1; i++) {
        if (m_data[i] == ch) {
            return i;
        }
    }

    return -1;
}

template <typename T>
void CircularQueue<T>::replace(T value, int index) {
    setInsertIndex(index);
    insert(value);
}