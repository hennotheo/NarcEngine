//
// Created by theoh on 27/02/2025.
//

#pragma once

namespace narclog
{
    template <typename T>
    concept MessageConcept =
        std::is_same_v<T, const char*> ||
        std::is_same_v<T, std::string> ||
        std::is_same_v<T, std::string&> ||
        std::is_same_v<T, const std::string&> ||
        std::is_same_v<T, std::string*> ||
        std::is_same_v<T, const std::string*>;
}
