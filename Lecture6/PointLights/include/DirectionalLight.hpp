#pragma once

#include <Light.hpp>

class DirectionalLight: public Light
{
public:
    DirectionalLight() = default;

    DirectionalLight(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity,
                     GLfloat x_dir, GLfloat y_dir, GLfloat z_dir, GLfloat _diffuse_intensity) noexcept;

    ~DirectionalLight();

    void use(GLuint color_id, GLuint ambient_intensity_id, GLuint diffuse_intensity_id, GLuint direction_id) const noexcept;

private:
    glm::vec3 direction{0.f, -1.f, 0.f};
};