#version 410

in vec2 texture_coordinates;
in vec3 normal;

out vec4 color;

struct DirectionalLight
{
    vec3 color;
    float ambient_intensity;
    vec3 direction;
    float diffuse_intensity;
};

uniform sampler2D the_texture;
uniform DirectionalLight directional_light;

void main()
{
    vec4 ambient_color = vec4(directional_light.color, 1.f) * directional_light.ambient_intensity;

    float diffuse_factor = max(dot(normalize(normal), normalize(directional_light.direction)), 0.f);
    vec4 diffuse_color = vec4(directional_light.color, 1.f) * directional_light.diffuse_intensity * diffuse_factor;

    color = texture(the_texture, texture_coordinates) * (ambient_color + diffuse_color);
}