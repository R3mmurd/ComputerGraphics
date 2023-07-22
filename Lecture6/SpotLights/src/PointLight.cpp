#include <PointLight.hpp>

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
                       GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat _a, GLfloat _b, GLfloat _c) noexcept
    : Light{red, green, blue, _ambient_intensity, _diffuse_intensity}, position{pos_x, pos_y, pos_z}, a{_a}, b{_b}, c{_c}
{

}

PointLight::~PointLight()
{
    
}

void PointLight::use(GLuint color_id, GLuint ambient_intensity_id, GLuint diffuse_intensity_id, GLuint position_id, GLuint a_id, GLuint b_id, GLuint c_id) const noexcept
{
    Light::use(color_id, ambient_intensity_id, diffuse_intensity_id);
    glUniform3f(position_id, position.x, position.y, position.z);
    glUniform1f(a_id, a);
    glUniform1f(b_id, b);
    glUniform1f(c_id, c);
}