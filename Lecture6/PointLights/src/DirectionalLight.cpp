#include <DirectionalLight.hpp>

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity,
                                   GLfloat x_dir, GLfloat y_dir, GLfloat z_dir, GLfloat _diffuse_intensity) noexcept
    : Light{red, green, blue, _ambient_intensity, _diffuse_intensity}, direction{x_dir, y_dir, z_dir}
{

}

DirectionalLight::~DirectionalLight()
{

}

void DirectionalLight::use(GLuint color_id, GLuint ambient_intensity_id, GLuint diffuse_intensity_id, GLuint direction_id) const noexcept
{
    Light::use(color_id, ambient_intensity_id, diffuse_intensity_id);
    glUniform3f(direction_id, direction.x, direction.y, direction.z);
}