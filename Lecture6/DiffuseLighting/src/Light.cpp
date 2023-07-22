#include <Light.hpp>

Light::Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity,
             GLfloat x_dir, GLfloat y_dir, GLfloat z_dir, GLfloat _diffuse_intensity) noexcept
    : color{red, green, blue}, ambient_intensity{_ambient_intensity},
      direction{x_dir, y_dir, z_dir}, diffuse_intensity{_diffuse_intensity}
{

}

Light::~Light()
{

}

void Light::use(GLuint ambient_intensity_id, GLuint ambient_color_id, GLuint diffuse_intensity_id, GLuint direction_id) const noexcept
{
    glUniform3f(ambient_color_id, color.x, color.y, color.z);
    glUniform1f(ambient_intensity_id, ambient_intensity);
    
    glUniform3f(direction_id, direction.x, direction.y, direction.z);
    glUniform1f(diffuse_intensity_id, diffuse_intensity);
}