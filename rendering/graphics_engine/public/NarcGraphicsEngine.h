//
// Created by theo on 5/10/26.
//


#pragma once

#include <pch.h>

#include "interfaces/IGraphicsEngine.h"
#include "interfaces/IGraphicsEngineBuilder.h"

namespace narc_engine {
    std::unique_ptr<IGraphicsEngineBuilder> createGraphicsEngineBuilder();
}