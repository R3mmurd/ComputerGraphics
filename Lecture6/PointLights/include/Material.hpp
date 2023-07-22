#pragma once

#include <GL/glew.h>

class Material
{
public:
    Material() = default;

    Material(GLfloat _specular_intensity, GLfloat _shininess) noexcept;

    ~Material();

    void use(GLuint specular_intensity_id, GLuint specular_shininess_id) const noexcept;

private:
    GLfloat specular_intensity{0.f};
    GLfloat shininess{0.f};
};