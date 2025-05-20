//
// Created by theoh on 3/4/2025.
//

#pragma once

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/scalar_constants.hpp>

#include "pch.h"

#define NARC_KB_SIZEOF(obj) (static_cast<double>(sizeof(obj)) / 1024.0)

#include "Transform.h"