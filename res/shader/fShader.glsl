#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 ourColor;
in vec3 normal;
in vec3 fragPos;

struct Material{

    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct Light{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    // Ambient
    vec4 ambient = vec4(light.ambient, 1.0f) * texture(material.diffuse, TexCoords);

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position-fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec4 diffuse = vec4(light.diffuse, 1.0f) * diff * texture(material.diffuse, TexCoords);

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec4 specular = vec4(light.specular, 1.0f) * spec * texture(material.specular, TexCoords);

    vec4 result = ambient + diffuse + specular;
    FragColor = vec4(result.rgb, 1.0f);
}