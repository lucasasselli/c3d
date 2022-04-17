#version 330 core

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 cameraPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main()
{
    vec3 objColor = vec3(0.5, 0.0, 0.0);
    vec3 norm = normalize(Normal);
    float specularStrength = 0.5;

    // Ambient
    vec3 ambient = vec3(0.0, 0.0, 0.0);

    // Diffuse
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular
    vec3 cameraDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(cameraDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objColor;
    FragColor = vec4(result, 1.0);
}