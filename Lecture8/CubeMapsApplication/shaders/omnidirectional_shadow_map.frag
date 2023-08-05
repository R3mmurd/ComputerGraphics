#version 410

in vec4 fragment_position;

uniform vec3 light_position;
uniform float far_plane;

void main()
{
    gl_FragDepth = length(fragment_position.xyz - light_position) / far_plane;
}