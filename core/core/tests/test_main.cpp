//
// Created by theoh on 3/23/2025.
//

#include <gtest/gtest.h>

#include <gmock/gmock.h>

#include "NarcCore.h"

class EventTest : public ::testing::Test
{
protected:
    narc_core::Event<void()> event; // Event sans arguments pour tester des callbacks simples.
};

// Test : Classe Event avec un handler sans arguments
TEST(EventTest, SubscriptionAndTriggerNoArgs) {
    narc_core::Event<> event;

    bool called = false;
    auto subscription = event.subscribe([&]() {
        called = true;
        });

    // Vérifie que le handler est appelé lors du déclenchement
    event.trigger();
    EXPECT_TRUE(called);
}

// Test : Classe Event avec un handler prenant un argument
TEST(EventTest, SubscriptionAndTriggerWithArgs) {
    narc_core::Event<int> event;

    int receivedValue = 0;
    auto subscription = event.subscribe([&](int value) {
        receivedValue = value;
        });

    // Déclenche l'événement avec une valeur
    event.trigger(42);
    EXPECT_EQ(receivedValue, 42);
}

// Test : Plusieurs abonnements
TEST(EventTest, MultipleSubscriptions) {
    narc_core::Event<> event;

    int callCount = 0;
    auto subscription1 = event.subscribe([&]() {
        callCount++;
        });
    auto subscription2 = event.subscribe([&]() {
        callCount++;
        });

    // Déclenche l'événement une fois
    event.trigger();
    EXPECT_EQ(callCount, 2);
}

// Test : Désabonnement explicite
TEST(EventTest, UnsubscribeExplicitly) {
    narc_core::Event<> event;

    bool called = false;
    auto subscription = event.subscribe([&]() {
        called = true;
        });

    subscription.unsubscribe();
    event.trigger();

    // Vérifie que le handler n'est pas appelé après désabonnement
    EXPECT_FALSE(called);
}

// Test : Désabonnement automatique via RAII
TEST(EventTest, UnsubscribeAutomatically) {
    narc_core::Event<> event;

    bool called = false;
    {
        auto subscription = event.subscribe([&]() {
            called = true;
            });
        // subscription se détruit ici
    }

    event.trigger();

    // Vérifie que le handler n'est pas appelé après destruction de l'abonnement
    EXPECT_FALSE(called);
}

// Test : Abonnement avec move semantics
TEST(EventTest, SubscriptionMoveSemantics) {
    narc_core::Event<> event;

    bool called = false;
    auto subscription1 = event.subscribe([&]() {
        called = true;
        });

    // Déplace l'abonnement
    auto subscription2 = std::move(subscription1);

    // Déclenche l'événement
    event.trigger();
    EXPECT_TRUE(called);
}

// Test : Désabonnement via move semantics
TEST(EventTest, UnsubscribeViaMovedSubscription) {
    narc_core::Event<> event;

    bool called = false;
    auto subscription1 = event.subscribe([&]() {
        called = true;
        });

    // Déplace l'abonnement
    auto subscription2 = std::move(subscription1);

    // Désabonne via le nouvel objet
    subscription2.unsubscribe();

    event.trigger();

    // Vérifie que le handler n'est pas appelé
    EXPECT_FALSE(called);
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}