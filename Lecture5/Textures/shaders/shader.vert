#version 410

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;

out vec4 vertex_color;
out vec2 texture_coordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
    vertex_color = vec4(clamp(pos, 0.f, 1.f), 1.f);
    texture_coordinates = tex;
}