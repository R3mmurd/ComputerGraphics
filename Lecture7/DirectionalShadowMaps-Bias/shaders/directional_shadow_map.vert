#version 410

layout (location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 directional_light_space_transform;

void main()
{
    gl_Position = directional_light_space_transform * model * vec4(pos, 1.0);
}