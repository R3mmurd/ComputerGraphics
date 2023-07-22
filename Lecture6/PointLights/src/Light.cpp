#include <Light.hpp>

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity) noexcept
    : color{red, green, blue}, ambient_intensity{_ambient_intensity}, diffuse_intensity{_diffuse_intensity}
{

}

Light::~Light()
{

}

void Light::use(GLuint color_id,GLuint ambient_intensity_id, GLuint diffuse_intensity_id) const noexcept
{
    glUniform3f(color_id, color.x, color.y, color.z);
    glUniform1f(ambient_intensity_id, ambient_intensity);
    glUniform1f(diffuse_intensity_id, diffuse_intensity);
}