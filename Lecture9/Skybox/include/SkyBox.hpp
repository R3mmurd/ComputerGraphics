#pragma once

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <Mesh.hpp>
#include <Shader.hpp>

class SkyBox
{
public:
    static const std::filesystem::path& vertex_shader_filename;
    static const std::filesystem::path& fragment_shader_filename;

    SkyBox() = default;

    SkyBox(const std::filesystem::path& root_path, const std::vector<std::filesystem::path>& face_filenames) noexcept;

    ~SkyBox();

    void render(const glm::mat4& view, const glm::mat4 projection) const noexcept;

private:
    std::shared_ptr<Mesh> mesh{nullptr};
    std::shared_ptr<Shader> shader{nullptr};
    
    GLuint texture_id{0};
    GLuint uniform_projection_id{0};
    GLuint uniform_view_id{0};
};
