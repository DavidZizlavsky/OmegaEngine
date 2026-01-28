#version 450 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

uniform mat4 u_ModelMatrix;

layout(std140, binding = 0) uniform FrameData
{
    mat4 u_ViewMatrix;
    mat4 u_ProjectionMatrix;
    mat4 u_ViewProjectionMatrix;
    vec3 u_CameraPosition;
};

out vec3 FragPos;
out vec3 Normal;

void main() {
    FragPos = vec3(u_ModelMatrix * vec4(inPosition, 1.0));
    Normal = mat3(transpose(inverse(u_ModelMatrix))) * inNormal;

    gl_Position = u_ViewProjectionMatrix * u_ModelMatrix * vec4(inPosition, 1.0);
}