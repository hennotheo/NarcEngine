#version 450

layout (binding = 0) uniform UniformBufferObject {
    mat4 Model[10];
    mat4 View;
    mat4 Proj;
} ubo;

layout (push_constant) uniform PushConstants {
    int objectIndex;
} pc;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inTexcoord;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 fragTexcoord;

void main()
{
    gl_Position = ubo.Proj * ubo.View * ubo.Model[pc.objectIndex] * vec4(inPosition, 1.0);
    fragColor = inColor;
    fragTexcoord = inTexcoord;
}