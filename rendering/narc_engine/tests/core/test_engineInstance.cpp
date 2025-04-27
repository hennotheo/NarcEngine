#include "test_includes.h"

INITIALISATION_TESTS(EngineBuilder_ValidationLayers_NoThrow)
{
    EXPECT_NO_THROW(EngineBuilder builder;
        builder.setValidationLayers(&g_validationLayers););
}

INITIALISATION_TESTS(EngineInstance_NoThrow)
{
    EngineBuilder builder;
    builder.setValidationLayers(&g_validationLayers); 

    EXPECT_NO_THROW(EngineInstance instance(&builder););
}