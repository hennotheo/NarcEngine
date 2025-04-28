//
// Created by theoh on 3/10/2025.
//

#pragma once

#define GETTER [[nodiscard]]

#define DEPRECATED [[deprecated]]
#define TEMP_CODE [[deprecated("Temporary code just for testing purposes")]]

#ifdef NARC_TEST_BUILD

#define PRIVATE_TESTABLE public
#define PROTECTED_TESTABLE public

#else

#define PRIVATE_TESTABLE private
#define PROTECTED_TESTABLE protected

#endif

#include "pch.h"

#include "Events.h"