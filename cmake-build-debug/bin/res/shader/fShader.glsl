#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 ourColor;
in vec3 normal;
in vec3 fragPos;

struct Material {
    sampler2D diffuse;
    sampler2D specular;

    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float constant;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float linear;
    float constant;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[4];
uniform SpotLight spotLight;
uniform vec3 viewPos;

vec3 calcDirLight(DirLight light, vec3 norm, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragpos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 fragpos, vec3 viewDir);

void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calcDirLight(dirLight, norm, viewDir);

    vec4 texColor = texture(material.diffuse, TexCoords);

    // все вершины квадрата
    for (int i = 0; i < 4; i++)
    result += calcPointLight(pointLights[i], norm, fragPos, viewDir);
//    result += calcSpotLight(spotLight, norm, fragPos, viewDir);

    FragColor = vec4(result, 1.0f);
}

vec3 calcDirLight(DirLight light, vec3 norm, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    float diff_koef = max(dot(norm, lightDir), 0.0f);

    // блик
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);


    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff_koef * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    return ambient + diffuse + specular;
}

vec3 calcPointLight(PointLight light, vec3 norm, vec3 fragpos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragpos);
    float diff_koef = max(dot(norm, lightDir), 0.0f);

    // блик
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float Distance = length(light.position - fragpos);
    float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff_koef * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}

vec3 calcSpotLight(SpotLight light, vec3 norm, vec3 fragpos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragpos);
    float diff_koef = max(dot(norm, lightDir), 0.0f);

    // блик
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    float Distance = length(light.position - fragpos);
    float attenuation = 1.0f / (light.constant + light.linear * Distance + light.quadratic * pow(Distance, 2));

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0f, 1.0f);

    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
    vec3 diffuse = light.diffuse * diff_koef * texture(material.diffuse, TexCoords).rgb;
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}