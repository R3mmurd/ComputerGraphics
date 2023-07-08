#version 410

layout(location = 0) in vec3 pos;

out vec4 vertex_color;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(pos.x, pos.y, pos.z, 1.0);
    vertex_color = vec4(clamp(pos, 0.f, 1.f), 1.f);
}