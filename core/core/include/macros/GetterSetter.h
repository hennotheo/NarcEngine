//
// Created by theoh on 26/05/2025.
//

#pragma once

#define GETTER [[nodiscard]] inline
#define NARC_GETTER(type, displayName, result) [[nodiscard]] inline type const get##displayName() const noexcept { return result; }
#define NARC_VIRTUAL_GETTER(type, displayName, result) [[nodiscard]] virtual type const get##displayName() const noexcept { return result; }
#define NARC_PURE_VIRTUAL_GETTER(type, displayName) [[nodiscard]] virtual type const get##displayName() const noexcept = 0;
#define NARC_OVERRIDE_GETTER(type, displayName, result) [[nodiscard]] inline type const get##displayName() const noexcept override { return result; }

#define SETTER inline