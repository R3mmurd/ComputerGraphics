#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <string_view>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <BSlogger.hpp>

#include <DirectionalLight.hpp>
#include <PointLight.hpp>
#include <SpotLight.hpp>

class Shader
{
public:
    static constexpr size_t MAX_POINT_LIGHTS{10};
    static constexpr size_t MAX_SPOT_LIGHTS{10};

    Shader() = default;

    Shader(const Shader& shader) = delete;

    Shader(Shader&& shader) = delete;

    ~Shader();

    Shader& operator = (const Shader& shader) = delete;

    Shader& operator = (Shader&& shader) = delete;

    static std::shared_ptr<Shader> create_from_strings(std::string_view vertex_shader_code, std::string_view geometry_shader_code, std::string_view fragment_shader_code) noexcept;

    static std::shared_ptr<Shader> create_from_files(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path) noexcept;

    static std::shared_ptr<Shader> create_from_files(std::filesystem::path vertex_shader_path, std::filesystem::path geometry_shader_path, std::filesystem::path fragment_shader_path) noexcept;

    GLuint get_uniform_projection_id() const noexcept { return uniform_projection_id; }

    GLuint get_uniform_view_id() const noexcept { return uniform_view_id; }

    GLuint get_uniform_model_id() const noexcept { return uniform_model_id; }

    GLuint get_uniform_eye_position_id() const noexcept { return uniform_eye_position_id; }

    GLuint get_uniform_specular_intensity_id() const noexcept { return uniform_specular_intensity_id; }

    GLuint get_uniform_specular_shininess_id() const noexcept { return uniform_specular_shininess_id; }

    GLuint get_uniform_omnidirectional_light_position_id() const noexcept { return uniform_omnidirectional_light_position_id; }

    GLuint get_uniform_far_plane_id() const noexcept { return uniform_far_plane_id; }

    void use() const noexcept;

    void set_directional_light(std::shared_ptr<DirectionalLight> light) const noexcept;
    
    void set_point_lights(const std::vector<std::shared_ptr<PointLight>>& lights, unsigned int texture_unit, size_t offset) const noexcept;

    void set_spot_lights(const std::vector<std::shared_ptr<SpotLight>>& lights, unsigned int texture_unit, size_t offset) const noexcept;

    void set_texture(GLenum texture_unit) const noexcept;

    void set_directional_shadow_map(GLenum texture_unit) const noexcept;

    void set_directional_light_space_transform(const glm::mat4& directional_light_space_transform) const noexcept;

    void set_omnidirectional_light_matrices(const std::vector<glm::mat4>& matrices) const noexcept;

private:
    void clear() noexcept;

    void create_program(std::string_view vertex_shader_code, std::string_view geometry_shader_code, std::string_view fragment_shader_code) noexcept;

    void create_shader(std::string_view shader_code, GLenum shader_type) noexcept;

    static std::string read_file(const std::filesystem::path& shader_path) noexcept;

    GLuint program_id{0};
    GLuint uniform_projection_id{0};
    GLuint uniform_view_id{0};
    GLuint uniform_model_id{0};
    GLuint uniform_eye_position_id{0};
    GLuint uniform_specular_intensity_id{0};
    GLuint uniform_specular_shininess_id{0};
    GLuint uniform_directional_light_space_transform_id{0};
    GLuint uniform_directional_shadow_map_id{0};
    GLuint uniform_texture_id{0};
    GLuint uniform_omnidirectional_light_position_id{0};
    GLuint uniform_far_plane_id{0};
    GLuint uniform_light_matrix_ids[OmnidirectionalShadowMap::NUM_FACES];
    
    struct
    {
        GLuint uniform_color_id;
        GLuint uniform_ambient_intensity_id;
        GLuint uniform_diffuse_intensity_id;
        GLuint uniform_direction_id;
    } uniform_directional_light;

    GLuint uniform_num_point_lights;

    struct
    {
        GLuint uniform_color_id;
        GLuint uniform_ambient_intensity_id;
        GLuint uniform_diffuse_intensity_id;
        GLuint uniform_position_id;
        GLuint uniform_a_id;
        GLuint uniform_b_id;
        GLuint uniform_c_id;
    } uniform_point_lights[MAX_POINT_LIGHTS];

    GLuint uniform_num_spot_lights;

    struct
    {
        GLuint uniform_color_id;
        GLuint uniform_ambient_intensity_id;
        GLuint uniform_diffuse_intensity_id;
        GLuint uniform_position_id;
        GLuint uniform_a_id;
        GLuint uniform_b_id;
        GLuint uniform_c_id;
        GLuint uniform_direction_id;
        GLuint uniform_edge_id;
    } uniform_spot_lights[MAX_SPOT_LIGHTS];

    struct
    {
        GLuint uniform_shadow_map_id;
        GLuint uniform_far_plane_id;
    } uniform_omnidirectional_shadow_maps[MAX_POINT_LIGHTS + MAX_SPOT_LIGHTS];
};
