//
// Created by theoh on 3/23/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NarcCore.h"

class EventTest : public ::testing::Test
{
protected:
    narc_core::Event<void()> event;
};

TEST(EventTest, SubscriptionAndTriggerNoArgs) {
    narc_core::Event<> event;

    bool called = false;
    auto subscription = event.subscribe([&]() {
        called = true;
        });

    event.trigger();
    EXPECT_TRUE(called);
}

TEST(EventTest, SubscriptionAndTriggerWithArgs) {
    narc_core::Event<int> event;

    int receivedValue = 0;
    auto subscription = event.subscribe([&](int value) {
        receivedValue = value;
        });

    event.trigger(42);
    EXPECT_EQ(receivedValue, 42);
}

TEST(EventTest, MultipleSubscriptions) {
    narc_core::Event<> event;

    int callCount = 0;
    auto subscription1 = event.subscribe([&]() {
        callCount++;
        });
    auto subscription2 = event.subscribe([&]() {
        callCount++;
        });

    event.trigger();
    EXPECT_EQ(callCount, 2);
}

TEST(EventTest, UnsubscribeExplicitly) {
    narc_core::Event<> event;

    bool called = false;
    auto subscription = event.subscribe([&]() {
        called = true;
        });

    subscription.unsubscribe();
    event.trigger();

    EXPECT_FALSE(called);
}

TEST(EventTest, UnsubscribeAutomatically) {
    narc_core::Event<> event;

    bool called = false;
    {
        auto subscription = event.subscribe([&]() {
            called = true;
            });
    }

    event.trigger();

    EXPECT_FALSE(called);
}

TEST(EventTest, SubscriptionMoveSemantics) {
    narc_core::Event<> event;

    bool called = false;
    auto subscription1 = event.subscribe([&]() {
        called = true;
        });

    auto subscription2 = std::move(subscription1);

    event.trigger();
    EXPECT_TRUE(called);
}

TEST(EventTest, UnsubscribeViaMovedSubscription) {
    narc_core::Event<> event;

    bool called = false;
    auto subscription1 = event.subscribe([&]() {
        called = true;
        });

    auto subscription2 = std::move(subscription1);

    subscription2.unsubscribe();

    event.trigger();
    
    EXPECT_FALSE(called);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}