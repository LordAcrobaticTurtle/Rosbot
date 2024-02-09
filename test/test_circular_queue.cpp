#include "gtest/gtest.h"

#include "data-structures/circular_queue.h"

#include <stdio.h>

class TestCircularQueue : public ::testing::Test {
    void SetUp() {

    }

    void TearDown() {

    }
};

class TestableCircularQueue : public CircularQueue {
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
    TestableCircularQueue buffer;
    int tailBeforeInsert = buffer.getTailPos();
    byte value = 0xFF;
    buffer.insert(value);

    int tailAfterInsert = buffer.getTailPos();
    // buffer.printQueue();

    EXPECT_EQ(tailAfterInsert, 0);
    EXPECT_EQ(buffer[tailAfterInsert], value);
}

TEST_F(TestCircularQueue, test_zeroes_on_construction) {
    TestableCircularQueue buffer;

    EXPECT_EQ(buffer.getTailPos(), 0);

    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        EXPECT_EQ(buffer[i], 0);
    }
}

TEST_F(TestCircularQueue, test_element_insert_overflows_correctly) {
    TestableCircularQueue buffer;

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
    TestableCircularQueue buffer;
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        buffer.insert(i);
    }
    // buffer.printQueue();
    EXPECT_EQ(buffer[-10], 10);
    
}

TEST_F(TestCircularQueue, test_read_once_valid_return) {
    TestableCircularQueue buffer;

    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        buffer.insert(i);
    }

    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        NextValue value = buffer.getNextValue();
        EXPECT_EQ(value.valid, true);
        EXPECT_EQ(value.value, i);
        EXPECT_EQ(value.index, i);
    }

    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        NextValue value = buffer.getNextValue();
        EXPECT_EQ(value.valid, false);
        EXPECT_EQ(value.value, 0);
        EXPECT_EQ(value.index, i);
    }
}

TEST_F(TestCircularQueue, test_copy_data) {
    TestableCircularQueue queue;
    const int maxSize = 16;
    for (int i = 0; i < maxSize; i++) {
        queue.insert(i);
    }

    byte buffer[maxSize];
    queue.copyData(buffer, 0, maxSize-1);
    // 0 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15
    
    for (byte i=0; i < maxSize; i++) {
        EXPECT_EQ(i, buffer[i]);
    }

    queue.setInsertIndex(240);
    for (int i = 240; i < 256; i++) {
        queue.insert(i);
    }

    queue.copyData(buffer, 240, 255);
    for (byte i = 0; i < maxSize; i++) {
        EXPECT_EQ(i+240, buffer[i]);
    }
}

TEST_F(TestCircularQueue, test_circ_queue_search_element_present_final_position) {
    TestableCircularQueue queue;

    char buffer[] = "Begin\n";
    queue.insert(buffer, strlen(buffer));
    EXPECT_EQ(queue.searchForCharacter('\n'), 5);
}

TEST_F(TestCircularQueue, test_circ_queue_search_element_present_mid_position) {
    TestableCircularQueue queue;

    char buffer[] = "Be\ngin";
    queue.insert(buffer, strlen(buffer));
    EXPECT_EQ(queue.searchForCharacter('\n'), 2);
}

TEST_F(TestCircularQueue, test_circ_queue_search_element_present_first_position) {
    TestableCircularQueue queue;

    char buffer[] = "\nBegin";
    queue.insert(buffer, strlen(buffer));
    EXPECT_EQ(queue.searchForCharacter('\n'), 0);
}

TEST_F(TestCircularQueue, test_circ_queue_search_element_evaporated) {
    TestableCircularQueue queue;

    char buffer[] = "Begin";
    queue.insert(buffer, strlen(buffer));
    EXPECT_EQ(queue.searchForCharacter('\n'), -1);
}

TEST_F(TestCircularQueue, test_replace_last_element) {
    TestableCircularQueue queue;
    char buffer[] = "Begin\n";
    queue.insert(buffer, strlen(buffer));
    queue.replace('0', queue.getTailPos());

    EXPECT_EQ(queue[queue.getTailPos()], '0');
    
}

TEST_F(TestCircularQueue, test_replace_first_element) {
    TestableCircularQueue queue;
    char buffer[] = "Begin\n";
    queue.insert(buffer, strlen(buffer));
    queue.replace('0', 0);
    
    EXPECT_EQ(queue[0], '0');
}





