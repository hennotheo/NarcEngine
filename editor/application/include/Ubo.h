//
// Created by theo on 12/20/25.
//

#pragma once

struct UniformBufferObject
{
    narc_math::Matrix4 model;
    glm::mat4 view;
    glm::mat4 proj;
};