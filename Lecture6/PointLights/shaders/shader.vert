#version 410

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 norm;

out vec2 texture_coordinates;
out vec3 normal;
out vec3 fragment_position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);
    
    texture_coordinates = tex;
    
    normal = mat3(transpose(inverse(model))) * norm;

    fragment_position = (model * vec4(pos.x, pos.y, pos.z, 1.0)).xyz;
}