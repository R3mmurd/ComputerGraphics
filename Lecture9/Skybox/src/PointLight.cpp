#include <PointLight.hpp>

PointLight::PointLight(GLuint shadow_width, GLuint shadow_height,
                       GLfloat near, GLfloat far,
                       GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
                       GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat _a, GLfloat _b, GLfloat _c) noexcept
    : Light{shadow_width, shadow_height, red, green, blue, _ambient_intensity, _diffuse_intensity}, far_plane{far}, position{pos_x, pos_y, pos_z}, a{_a}, b{_b}, c{_c}
{
    float aspect = float(shadow_width) / float(shadow_height);
    projection = glm::perspective(glm::radians(90.f), aspect, near, far);
    shadow_map = std::make_shared<OmnidirectionalShadowMap>();
    shadow_map->init(shadow_width, shadow_height);
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

std::vector<glm::mat4> PointLight::get_light_transforms() const noexcept
{
    return std::vector<glm::mat4>{{
        // +x, -x
        projection * glm::lookAt(position, position + glm::vec3{1.f, 0.f, 0.f}, glm::vec3{0.f, -1.f, 0.f}),
        projection * glm::lookAt(position, position + glm::vec3{-1.f, 0.f, 0.f}, glm::vec3{0.f, -1.f, 0.f}),
        // +y, -y
        projection * glm::lookAt(position, position + glm::vec3{0.f, 1.f, 0.f}, glm::vec3{0.f, 0.f, 1.f}),
        projection * glm::lookAt(position, position + glm::vec3{0.f, -1.f, 0.f}, glm::vec3{0.f, 0.f, -1.f}),
        // +z, -z
        projection * glm::lookAt(position, position + glm::vec3{0.f, 0.f, 1.f}, glm::vec3{0.f, -1.f, 0.f}),
        projection * glm::lookAt(position, position + glm::vec3{0.f, 0.f, -1.f}, glm::vec3{0.f, -1.f, 0.f})
    }};
}