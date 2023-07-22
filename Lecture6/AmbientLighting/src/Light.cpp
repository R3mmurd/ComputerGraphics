#include <Light.hpp>

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity) noexcept
    : color{red, green, blue}, ambient_intensity{_ambient_intensity}
{

}

Light::~Light()
{

}

void Light::use(GLuint ambient_intensity_id, GLuint ambient_color_id) const noexcept
{
    glUniform3f(ambient_color_id, color.x, color.y, color.z);
    glUniform1f(ambient_intensity_id, ambient_intensity);
}