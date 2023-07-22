#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light() = default;

    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity) noexcept;

    virtual ~Light();

    void use(GLuint color_id,GLuint ambient_intensity_id, GLuint diffuse_intensity_id) const noexcept;

protected:
    glm::vec3 color{1.f, 1.f, 1.f};
    GLfloat ambient_intensity{1.f};
    GLfloat diffuse_intensity{0.f};
};