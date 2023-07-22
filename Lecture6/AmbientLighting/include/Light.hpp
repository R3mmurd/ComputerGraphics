#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light() = default;

    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity) noexcept;

    ~Light();

    void use(GLuint ambient_intensity_id, GLuint ambient_color_id) const noexcept;

private:
    glm::vec3 color{1.f, 1.f, 1.f};
    GLfloat ambient_intensity{1.f};
};