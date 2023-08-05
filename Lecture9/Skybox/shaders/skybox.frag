#version 410

in vec3 texture_coordinates;

out vec4 color;

uniform samplerCube skybox;

void main()
{
    color = texture(skybox, texture_coordinates);
}