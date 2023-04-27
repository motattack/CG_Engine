#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 ourColor;

uniform bool isTexture;
uniform vec3 colors;

uniform sampler2D container_texture;
uniform sampler2D face_texture;

uniform float alpha;

void main()
{
    if (isTexture)
        FragColor = mix(texture(container_texture, TexCoords), texture(face_texture, TexCoords), 0.2f);
    else
        FragColor = vec4(colors, 1.0f);
}