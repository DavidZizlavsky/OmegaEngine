#version 450 core

layout(location = 0) in vec3 inPosition;

uniform mat4 u_ModelMatrix;

layout(std140, binding = 0) uniform FrameData
{
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
};

void main() {
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_ModelMatrix * vec4(inPosition, 1.0);
}