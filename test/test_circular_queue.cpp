#include "gtest/gtest.h"

#include "data-structures/circular_queue.h"

#include <stdio.h>

class TestCircularQueue : public ::testing::Test {
    void SetUp() {

    }

    void TearDown() {

    }
};

class testCircularQueue : public CircularQueue {
    public:
        void printQueue() {
            printf("[ ");
            for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
                printf("%x, ", m_data[i]);
            }
            printf(" ]");
        }
};


TEST_F(TestCircularQueue, test_insert_element_increments) {
// Buffer is initially empty, inserting into empty queue should produce a tail at 0
    testCircularQueue buffer;
    int tailBeforeInsert = buffer.getTailPos();
    byte value = 0xFF;
    buffer.insert(value);

    int tailAfterInsert = buffer.getTailPos();
    // buffer.printQueue();

    EXPECT_EQ(tailAfterInsert, 0);
    EXPECT_EQ(buffer[tailAfterInsert], value);
}

TEST_F(TestCircularQueue, test_zeroes_on_construction) {
    testCircularQueue buffer;

    EXPECT_EQ(buffer.getTailPos(), 0);

    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        EXPECT_EQ(buffer[i], 0);
    }
}

TEST_F(TestCircularQueue, test_element_insert_overflows_correctly) {
    testCircularQueue buffer;

    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        buffer.insert(0xFF);    
    }

    int finalPosInBuffer = buffer.getTailPos();
    EXPECT_EQ(finalPosInBuffer, 255);

    buffer.insert(0x1);

    EXPECT_EQ(buffer[0], 0x1);
    EXPECT_EQ(buffer[finalPosInBuffer + 1], 0x1);
    // buffer.printQueue();
}

/**
 * @brief Tests that a negative index doesn't crash the code. Expected behaviour is to wrap index.
 */
TEST_F(TestCircularQueue, test_negative_index) {
    testCircularQueue buffer;
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        buffer.insert(i);
    }
    // buffer.printQueue();
    EXPECT_EQ(buffer[-10], 10);
    
}


