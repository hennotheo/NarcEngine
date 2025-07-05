//
// Created by theoh on 3/10/2025.
//

#pragma once

#define QUERY [[nodiscard]]

#define NARC_MUTABLE_THIS(type) const_cast<type*>(this)

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

#include "macros/Format.h"
#include "macros/GetterSetter.h"

#include "Events.h"

#include "interfaces/IGetter.h"
#include "interfaces/IInitialisable.h"
#include "interfaces/ILoadable.h"

#define DELETE_PTR_ARRAY(array) for (auto& ptr : array)\
    {\
        delete ptr;\
        ptr = nullptr;\
    }

#define NARC_DECL_DEPENDENCY_INJECTION(name, dependencyType) \
    inline void inject##name##Dependency(dependencyType& dependency)