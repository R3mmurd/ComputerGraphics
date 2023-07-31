#version 410

in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;
in vec4 directional_light_space_pos;

out vec4 color;

const int MAX_POINT_LIGHTS = 10;
const int MAX_SPOT_LIGHTS = 10;

struct Light
{
    vec3 color;
    float ambient_intensity;
    float diffuse_intensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;
};

struct PointLight
{
    Light base;
    vec3 position;
    float a;
    float b;
    float c;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material
{
    float specular_intensity;
    float shininess;
};

uniform DirectionalLight directional_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int num_point_lights;
uniform SpotLight spot_lights[MAX_SPOT_LIGHTS];
uniform int num_spot_lights;

uniform sampler2D the_texture;
uniform sampler2D directional_shadow_map;

uniform Material material;

uniform vec3 eye_position;

float calculate_directional_shadow_factor(DirectionalLight light)
{
    vec3 projection_coordinates = directional_light_space_pos.xyz / directional_light_space_pos.w;
    projection_coordinates = projection_coordinates * 0.5 + 0.5;

    float current = projection_coordinates.z;
    
    vec3 norm = normalize(normal);
    vec3 light_dir = normalize(directional_light.direction);

    float bias = max(0.05 * (1.0 - dot(normal, light_dir)), 0.0005);

    float shadow = 0.0;

    vec2 texel_size = 1.0 / textureSize(directional_shadow_map, 0);

    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(directional_shadow_map, projection_coordinates.xy + vec2(x, y) * texel_size).r;
            shadow += current - bias > pcf_depth ? 1.0 : 0.0;
        }
    }

    shadow /= 9;

    if (projection_coordinates.z > 1.0)
    {
        shadow = 0.0;
    }

    return shadow;
}

vec4 calculate_light_by_direction(Light light, vec3 direction, float shadow_factor)
{
    vec4 ambient_color = vec4(light.color, 1.f) * light.ambient_intensity;

    float diffuse_factor = max(dot(normalize(normal), normalize(direction)), 0.f);
    vec4 diffuse_color = vec4(light.color, 1.f) * light.diffuse_intensity * diffuse_factor;

    vec4 specular_color = vec4(0, 0, 0, 0);

    if (diffuse_factor > 0.f)
    {
        vec3 fragment_to_eye = normalize(eye_position - fragment_position);
        vec3 reflected_vertex = normalize(reflect(direction, normalize(normal)));
        float specular_factor = dot(fragment_to_eye, reflected_vertex);

        if (specular_factor > 0.f)
        {
            specular_factor = pow(specular_factor, material.shininess);
            specular_color = vec4(light.color * material.specular_intensity * specular_factor, 1.f);
        }
    }

    return ambient_color + (1.f - shadow_factor) * (diffuse_color + specular_color);
}

vec4 calculate_directional_light()
{
    float shadow_factor = calculate_directional_shadow_factor(directional_light);
    return calculate_light_by_direction(directional_light.base, directional_light.direction, shadow_factor);
}

vec4 calculate_point_light(PointLight light)
{
    vec3 direction = fragment_position - light.position;
    float distance = length(direction);
    direction = normalize(direction);
    vec4 color = calculate_light_by_direction(light.base, direction, 0.f);
    float attenuation = light.a * distance * distance +
                        light.b * distance +
                        light.c;
    return (color / attenuation);
}

vec4 calculate_point_lights()
{
	vec4 total_color = vec4(0, 0, 0, 0);

	for (int i = 0; i < num_point_lights; ++i)
	{
		total_color += calculate_point_light(point_lights[i]);
	}
	
	return total_color;
}

vec4 calculate_spot_light(SpotLight light)
{
    vec3 ray_direction = normalize(fragment_position - light.base.position);
    float factor = dot(ray_direction, light.direction);

    if (factor > light.edge)
    {
        return calculate_point_light(light.base) * (1.f - (1.f - factor) * (1.f / (1.f - light.edge)));
    }

    return vec4(0.f, 0.f, 0.f, 0.f);
}

vec4 calculate_spot_lights()
{
    vec4 total_color = vec4(0, 0, 0, 0);

	for (int i = 0; i < num_spot_lights; ++i)
	{
		total_color += calculate_spot_light(spot_lights[i]);
	}
	
	return total_color;
}

void main()
{
    vec4 final_color = calculate_directional_light() + calculate_point_lights() + calculate_spot_lights();
    color = texture(the_texture, texture_coordinates) * final_color;
}