#version 410

in vec2 texture_coordinates;
in vec3 normal;
in vec3 fragment_position;

out vec4 color;

const int MAX_POINT_LIGHTS = 10;

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

struct Material
{
    float specular_intensity;
    float shininess;
};

uniform DirectionalLight directional_light;
uniform PointLight point_lights[MAX_POINT_LIGHTS];
uniform int num_point_lights;

uniform sampler2D the_texture;
uniform Material material;

uniform vec3 eye_position;

vec4 calculate_light_by_direction(Light light, vec3 direction)
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

    return ambient_color + diffuse_color + specular_color;
}

vec4 calculate_directional_light()
{
    return calculate_light_by_direction(directional_light.base, directional_light.direction);
}

vec4 calculate_point_lights()
{
	vec4 total_color = vec4(0, 0, 0, 0);

	for (int i = 0; i < num_point_lights; ++i)
	{
		vec3 direction = fragment_position - point_lights[i].position;
		float distance = length(direction);
		direction = normalize(direction);
		vec4 color = calculate_light_by_direction(point_lights[i].base, direction);
		float attenuation = point_lights[i].a * distance * distance +
							point_lights[i].b * distance +
							point_lights[i].c;
		
		total_color += (color / attenuation);
	}
	
	return total_color;
}

void main()
{
    vec4 final_color = calculate_directional_light() + calculate_point_lights();
    color = texture(the_texture, texture_coordinates) * final_color;
}