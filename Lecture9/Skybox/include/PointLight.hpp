#pragma once

#include <Light.hpp>
#include <OmnidirectionalShadowMap.hpp>

class PointLight: public Light
{
public:
    PointLight() = default;

    PointLight(GLuint shadow_width, GLuint shadow_height,
               GLfloat near, GLfloat far,
               GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
               GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat _a, GLfloat _b, GLfloat _c) noexcept;

    ~PointLight();

    void use(GLuint color_id, GLuint ambient_intensity_id, GLuint diffuse_intensity_id, GLuint position_id, GLuint a_id, GLuint b_id, GLuint c_id) const noexcept;

    glm::mat4 get_light_transform() const noexcept { return glm::mat4{}; }

    std::vector<glm::mat4> get_light_transforms() const noexcept;

    const glm::vec3& get_position() const noexcept { return position; }

    GLfloat get_far_plane() const noexcept { return far_plane; }

protected:
    GLfloat far_plane{0};
    glm::vec3 position{0.f, 0.f, 0.f};
    GLfloat a; // Quadratic component in the equation
    GLfloat b; // Linear component in the equation
    GLfloat c; // Constant component in the equation
};
