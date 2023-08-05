#version 410

in vec4 fragment_position;

uniform vec3 light_position;
uniform float far_plane;

void main()
{
    float distance = length(fragment_position.xyz - light_position);
    distance = distance / far_plane;
    gl_FragDepth = distance;
}