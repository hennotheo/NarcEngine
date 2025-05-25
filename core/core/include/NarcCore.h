//
// Created by theoh on 3/10/2025.
//

#pragma once

#define QUERY [[nodiscard]]
#define GETTER [[nodiscard]] inline
#define VIRTUAL_GETTER [[nodiscard]] virtual
#define SETTER inline

#define DEPRECATED [[deprecated("Deprecated. This code will be removed in the future.")]]
#define TEMP_CODE [[deprecated("Temporary code just for testing purposes")]]

#ifdef NARC_TEST_BUILD

#define PRIVATE_TESTABLE public
#define PROTECTED_TESTABLE public

#else

#define PRIVATE_TESTABLE private
#define PROTECTED_TESTABLE protected

#endif

#include "pch.h"

#include "utils/Format.h"

#include "Events.h"

#include "interfaces/IGetter.h"
#include "interfaces/IInitialisable.h"
#include "interfaces/ILoadable.h"

#define DELETE_PTR_ARRAY(array) \
    for (auto& ptr : array)\
    {\
        delete ptr;\
        ptr = nullptr;\
    }