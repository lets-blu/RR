#include "gtest/gtest.h"
#include "messagequeue.h"

class MessageQueueTest : public ::testing::Test
{
protected:
    MessageQueue queue;

    const uint8_t QUEUE_LENGTH = 8;
    const uint8_t ITEM_SIZE = sizeof(int);

    void SetUp()
    {
        queue = newMessageQueue(QUEUE_LENGTH, ITEM_SIZE);
    }

    void TearDown()
    {
        deleteMessageQueue(&queue);
    }
};

TEST_F(MessageQueueTest, enMessageQueue)
{
    // test on deMessageQueue, peekMessageQueue and getMessageQueueItemCount
}

TEST_F(MessageQueueTest, deMessageQueue)
{
    int item = 0;

    for (int i = 0; i < QUEUE_LENGTH; i++)
    {
        enMessageQueue(&queue, &i);
    }

    for (int i = 0; i < QUEUE_LENGTH; i++)
    {
        deMessageQueue(&queue, &item);
        EXPECT_EQ(i, item);
    }
}

TEST_F(MessageQueueTest, peekMessageQueue)
{
    int item = 0;

    for (int i = 0; i < QUEUE_LENGTH; i++)
    {
        enMessageQueue(&queue, &i);
    }

    for (int i = 0; i < QUEUE_LENGTH; i++)
    {
        peekMessageQueue(&queue, &item);
        EXPECT_EQ(0, item);
    }
}

TEST_F(MessageQueueTest, getMessageQueueItemCount)
{
    for (int i = 0; i < QUEUE_LENGTH; i++)
    {
        enMessageQueue(&queue, &i);
        EXPECT_EQ(i + 1, getMessageQueueItemCount(&queue));
    }
}
