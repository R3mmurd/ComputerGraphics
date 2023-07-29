#include <DirectionalLight.hpp>

DirectionalLight::DirectionalLight(GLuint shadow_width, GLuint shadow_height,
                                   GLfloat red, GLfloat green, GLfloat blue,
                                   GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
                                   GLfloat x_dir, GLfloat y_dir, GLfloat z_dir) noexcept
    : Light{shadow_width, shadow_height, red, green, blue, _ambient_intensity, _diffuse_intensity}, direction{x_dir, y_dir, z_dir}
{
    projection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 100.0f);
}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::use(GLuint color_id, GLuint ambient_intensity_id, GLuint diffuse_intensity_id, GLuint direction_id) const noexcept
{
    Light::use(color_id, ambient_intensity_id, diffuse_intensity_id);
    glUniform3f(direction_id, direction.x, direction.y, direction.z);
}

glm::mat4 DirectionalLight::get_light_transform() const noexcept
{
    return projection * glm::lookAt(-direction, glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f});
}