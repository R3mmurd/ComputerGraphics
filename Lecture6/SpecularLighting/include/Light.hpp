#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
    Light() = default;

    Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity,
          GLfloat x_dir, GLfloat y_dir, GLfloat z_dir, GLfloat _diffuse_intensity) noexcept;

    ~Light();

    void use(GLuint ambient_intensity_id, GLuint ambient_color_id, GLuint diffuse_intensity_id, GLuint direction_id) const noexcept;

private:
    glm::vec3 color{1.f, 1.f, 1.f};
    GLfloat ambient_intensity{1.f};

    glm::vec3 direction{0.f, -1.f, 0.f};
    GLfloat diffuse_intensity{0.f};
};