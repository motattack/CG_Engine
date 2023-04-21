#version 330 core

out vec4 FragColor;

in vec3 curColor;

uniform float x_color;

void main()
{
    FragColor = vec4(x_color * curColor.x, curColor.y, curColor.z, 1.0f);
}