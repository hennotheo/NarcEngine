//
// Created by theoh on 26/05/2025.
//

#pragma once

// Macro de base corrigée
#define NARC_GETTER_MACRO_BASE(prefix, type, displayName, end, ...) \
[[nodiscard]] inline type get##displayName(__VA_ARGS__) const noexcept end

#define NARC_GETTER_BASE(type, displayName, result, ...) \
NARC_GETTER_MACRO_BASE(, type, displayName, { return result; }, __VA_ARGS__)

// Getters standards
#define NARC_GETTER(type, displayName, result, ...) \
[[nodiscard]] inline type displayName(__VA_ARGS__) const noexcept { return result; }

#define NARC_VIRTUAL_GETTER(type, displayName, result, ...) \
[[nodiscard]] virtual type displayName(__VA_ARGS__) const noexcept { return result; }

// Pure virtual getter corrigé (retire les parenthèses en trop)
#define NARC_PURE_VIRTUAL_GETTER(type, displayName, ...) \
[[nodiscard]] virtual type displayName(__VA_ARGS__) const noexcept = 0;

#define NARC_OVERRIDE_GETTER(type, displayName, result, ...) \
[[nodiscard]] inline type displayName(__VA_ARGS__) const noexcept override { return result; }

// Bool getters corrigés (retire "inline" des virtuelles)
#define NARC_BOOL_GETTER(displayName, result, ...) \
[[nodiscard]] inline bool displayName(__VA_ARGS__) const noexcept { return result; }

#define NARC_PURE_VIRTUAL_BOOL_GETTER(displayName, ...) \
[[nodiscard]] virtual bool displayName(__VA_ARGS__) const noexcept = 0;

#define NARC_VIRTUAL_BOOL_GETTER(displayName, result, ...) \
[[nodiscard]] virtual bool displayName(__VA_ARGS__) const noexcept { return result; }

#define NARC_OVERRIDE_BOOL_GETTER(displayName, result, ...) \
[[nodiscard]] inline bool displayName(__VA_ARGS__) const noexcept override { return result; }

#define SETTER inline
