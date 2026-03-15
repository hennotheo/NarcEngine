//
// Created by theo on 12/20/25.
//

#pragma once

struct UniformBufferObject
{
    narc_math::Matrix4 model;
    narc_math::Matrix4 view;
    narc_math::Matrix4 proj;
};