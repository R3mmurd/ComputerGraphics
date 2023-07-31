#include <SpotLight.hpp>


SpotLight::SpotLight(GLuint shadow_width, GLuint shadow_height,
                     GLfloat red, GLfloat green, GLfloat blue, GLfloat _ambient_intensity, GLfloat _diffuse_intensity,
                     GLfloat pos_x, GLfloat pos_y, GLfloat pos_z, GLfloat _a, GLfloat _b, GLfloat _c,
                     GLfloat dir_x, GLfloat dir_y, GLfloat dir_z, GLfloat _edge) noexcept
    : PointLight{shadow_width, shadow_height, red, green, blue, _ambient_intensity, _diffuse_intensity, pos_x, pos_y, pos_z, _a, _b, _c},
      direction{dir_x, dir_y, dir_z}, edge{_edge}
{
    direction = glm::normalize(direction);
    proc_edge = cosf(glm::radians(edge));
}

SpotLight::~SpotLight()
{

}

void SpotLight::use(GLuint color_id, GLuint ambient_intensity_id, GLuint diffuse_intensity_id,
                    GLuint position_id, GLuint a_id, GLuint b_id, GLuint c_id,
                    GLuint direction_id, GLuint edge_id) const noexcept
{
    PointLight::use(color_id, ambient_intensity_id, diffuse_intensity_id, position_id, a_id, b_id, c_id);
    glUniform3f(direction_id, direction.x, direction.y, direction.z);
    glUniform1f(edge_id, proc_edge);
}

void SpotLight::set(const glm::vec3& pos, const glm::vec3& dir) noexcept
{
    position = pos;
    direction = dir;
}