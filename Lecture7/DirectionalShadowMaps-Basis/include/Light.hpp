#pragma once

#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <ShadowMap.hpp>

class Light
{
public:
    Light() = default;

    Light(GLuint shadow_width, GLuint shadow_height, GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity) noexcept;

    virtual ~Light();

    void use(GLuint color_id,GLuint ambient_intensity_id, GLuint diffuse_intensity_id) const noexcept;

    virtual glm::mat4 get_light_transform() const noexcept = 0;

    std::shared_ptr<ShadowMap> get_shadow_map() const noexcept { return shadow_map; }

protected:
    glm::vec3 color{1.f, 1.f, 1.f};
    GLfloat ambient_intensity{1.f};
    GLfloat diffuse_intensity{0.f};
    glm::mat4 projection;
    std::shared_ptr<ShadowMap> shadow_map{nullptr};
};