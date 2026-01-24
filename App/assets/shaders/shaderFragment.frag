#version 450 core
layout(location = 0) out vec4 outColor;

uniform vec4 u_Color;

in vec3 FragPos;
in vec3 Normal;

void main() {
    vec3 lightPos = vec3(1.0, 3.0, 1.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 viewPos = vec3(1.0, 2.0, 5.0);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor * u_Color.rgb;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * u_Color.rgb;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = ambient + diffuse + specular;

    outColor = vec4(result, 1.0);

    // Normal visualizer:
    //outColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);
}