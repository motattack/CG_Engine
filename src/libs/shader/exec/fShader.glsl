#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 curColor;

uniform vec3 colors;
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords);
}