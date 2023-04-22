#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 ourColor;

uniform vec3 colors;

uniform sampler2D texture1;
uniform sampler2D texture2;

uniform float alpha;

void main()
{
    FragColor = mix(texture(texture1, TexCoords), texture(texture2, TexCoords), 0.0f);
}