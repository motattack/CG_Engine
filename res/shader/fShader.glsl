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
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float quadratic;
    float linear;
    float constant;

    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rbg;

    // Diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rbg;

    // Specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    // Soft Edges
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

    diffuse *= intensity;
    specular *= intensity;

    // Attenuation
    float Distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
}