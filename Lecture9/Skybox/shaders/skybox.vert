#version 410

layout (location = 0) in vec3 pos;

out vec3 texture_coordinates;

uniform mat4 view;
uniform mat4 projection;

void main()
{
    texture_coordinates = pos;
    gl_Position = projection * view * vec4(pos, 1.0);
}