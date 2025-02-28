//
// Created by theoh on 27/02/2025.
//

#pragma once

namespace narclog
{
    template <typename T>
    concept ArithmeticConcept = std::is_arithmetic_v<T>;

    template <typename T>
    concept MessageConcept =
        std::is_same_v<std::decay_t<T>, std::string> ||
        std::is_same_v<std::decay_t<T>, const char*>;

    template <typename T>
    concept LogConcept =
        MessageConcept<T> ||
        ArithmeticConcept<T>;
}
