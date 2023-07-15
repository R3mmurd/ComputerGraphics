#version 410

in vec4 vertex_color;
in vec2 texture_coordinates;

out vec4 color;

uniform sampler2D the_texture;

void main()
{
    color = texture(the_texture, texture_coordinates) * vertex_color;
}