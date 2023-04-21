#version 330 core

out vec4 FragColor;

in vec3 curColor;

uniform vec3 colors;

void main()
{
    FragColor = vec4(colors, 1.0f);
}