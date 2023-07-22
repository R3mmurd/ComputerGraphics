#pragma once

#include <Light.hpp>

class PointLight: public Light
{
public:
    PointLight() = default;

    PointLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
               GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat _a, GLfloat _b, GLfloat _c) noexcept;

    ~PointLight();

    void use(GLuint color_id, GLuint ambient_intensity_id, GLuint diffuse_intensity_id, GLuint position_id, GLuint a_id, GLuint b_id, GLuint c_id) const noexcept;

private:
    glm::vec3 position{0.f, 0.f, 0.f};
    GLfloat a; // Quadratic component in the equation
    GLfloat b; // Linear component in the equation
    GLfloat c; // Constant component in the equation
};
