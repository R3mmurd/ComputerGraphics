#pragma once

#include <PointLight.hpp>

class SpotLight: public PointLight
{
public:
    SpotLight() = default;

    SpotLight(GLuint shadow_width, GLuint shadow_height,
              GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
              GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat _a, GLfloat _b, GLfloat _c,
              GLfloat dir_x, GLfloat dir_y, GLfloat dir_z, GLfloat _edge) noexcept;

    ~SpotLight();

    void use(GLuint color_id, GLuint ambient_intensity_id, GLuint diffuse_intensity_id,
             GLuint position_id, GLuint a_id, GLuint b_id, GLuint c_id,
             GLuint direction_id, GLuint edge_id) const noexcept;

    void set(const glm::vec3& pos, const glm::vec3& dir) noexcept;

private:
    glm::vec3 direction;
    GLfloat edge;
    GLfloat proc_edge;
};