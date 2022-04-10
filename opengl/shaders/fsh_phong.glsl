#version 330 core

uniform vec3 lightPos;
uniform vec3 lightColor;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

void main()
{
    vec3 ambient = vec3(0.0, 0.0, 0.0);
    vec3 objColor = vec3(0.5, 0.0, 0.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (ambient + diffuse) * objColor;
    FragColor = vec4(result, 1.0);
}