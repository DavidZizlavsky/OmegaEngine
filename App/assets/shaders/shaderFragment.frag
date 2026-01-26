#version 450 core
layout(location = 0) out vec4 outColor;

struct PointLight
{
    vec4 position;
    vec4 color;
};

layout(std140, binding = 1) uniform LightData
{
    vec4 ambientColor;
    int lightCount;

    PointLight lights[16];
};

uniform vec4 u_Color;

in vec3 FragPos;
in vec3 Normal;

void main() {
    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * ambientColor.rgb * u_Color.rgb;

    vec3 result = vec3(0.0);
    result += ambient;

    for (int i = 0; i < lightCount; i++) 
    {
        vec3 lightPos = lights[i].position.xyz;
        vec3 lightColor = lights[i].color.rgb;
        float intensity = lights[i].color.a;
        vec3 viewPos = vec3(1.0, 2.0, 5.0);

        // diffuse
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * u_Color.rgb * intensity;

        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
        vec3 specular = spec * lightColor * intensity;

        result += diffuse + specular;

        
    }

    outColor = vec4(result, 1.0);

    // Normal visualizer:
    //outColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);
}