//
// Created by theoh on 3/10/2025.
//

#pragma once

#define GETTER [[nodiscard]]

#define NGETTER(type, name) \
    private: \
        type m_##name; \
    public: \
        GETTER type get##name() const { return m_##name; }

#define DEPRECATED [[deprecated]]