#version 410

in vec2 texture_coordinates;

out vec4 color;

struct DirectionalLight
{
    vec3 color;
    float ambient_intensity;
};

uniform sampler2D the_texture;
uniform DirectionalLight directional_light;

void main()
{
    vec4 ambient_color = vec4(directional_light.color, 1.f) * directional_light.ambient_intensity;

    color = texture(the_texture, texture_coordinates) * ambient_color;
}