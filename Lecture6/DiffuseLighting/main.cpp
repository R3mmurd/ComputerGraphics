#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Camera.hpp>
#include <Light.hpp>
#include <Mesh.hpp>
#include <Shader.hpp>
#include <Texture.hpp>
#include <Window.hpp>

namespace fs = std::filesystem;

struct Data
{
    static std::vector<std::shared_ptr<Mesh>> mesh_list;
    static std::vector<std::shared_ptr<Shader>> shader_list;
    static const fs::path root_path;
    static const fs::path vertex_shader_path;
    static const fs::path fragment_shader_path;
};

std::vector<std::shared_ptr<Mesh>> Data::mesh_list{};
std::vector<std::shared_ptr<Shader>> Data::shader_list{};

const fs::path Data::root_path{fs::path{__FILE__}.parent_path()};
const fs::path Data::vertex_shader_path{Data::root_path / "shaders" / "shader.vert"};
const fs::path Data::fragment_shader_path{Data::root_path / "shaders" / "shader.frag"};

float to_radian(float degrees)
{
    return degrees * M_PI / 180.f;
}

void calculate_average_normals(const std::vector<unsigned int>& indices, std::vector<GLfloat>& vertices, size_t vertex_length, size_t normal_offset) noexcept
{
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        size_t idx0 = indices[i]     * vertex_length;
        size_t idx1 = indices[i + 1] * vertex_length;
        size_t idx2 = indices[i + 2] * vertex_length;

        glm::vec3 v1{vertices[idx1] - vertices[idx0], vertices[idx1 + 1] - vertices[idx0 + 1], vertices[idx1 + 2] - vertices[idx0 + 2]};
		glm::vec3 v2{vertices[idx2] - vertices[idx0], vertices[idx2 + 1] - vertices[idx0 + 1], vertices[idx2 + 2] - vertices[idx0 + 2]};
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

        idx0 += normal_offset;
        idx1 += normal_offset;
        idx2 += normal_offset;

        vertices[idx0]     += normal.x;
        vertices[idx0 + 1] += normal.y;
        vertices[idx0 + 2] += normal.z;

        vertices[idx1]     += normal.x;
        vertices[idx1 + 1] += normal.y;
        vertices[idx1 + 2] += normal.z;

        vertices[idx2]     += normal.x;
        vertices[idx2 + 1] += normal.y;
        vertices[idx2 + 2] += normal.z;
    }

    for (size_t i = 0; i < vertices.size() / vertex_length; ++i)
    {
        size_t j = i * vertex_length + normal_offset;
        glm::vec3 normal{vertices[j], vertices[j + 1], vertices[j + 2]};
        normal = glm::normalize(normal);

        vertices[j]     = normal.x; 
        vertices[j + 1] = normal.y;
        vertices[j + 2] = normal.z;
    }
}

void specify_vertices() noexcept
{
    std::vector<unsigned int> indices{
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    std::vector<GLfloat> vertices{
    //    x     y    z    u    v    nx   ny   nz
        -1.f, -1.f, 0.f, 0.f,  0.f, 0.f, 0.f, 0.f,
        0.f, -1., 1.f,   0.5f, 0.f, 0.f, 0.f, 0.f,
        1.f, -1.f, 0.f,  1.f,  0.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f,   0.5f, 1.f, 0.f, 0.f, 0.f
    };

    calculate_average_normals(indices, vertices, 8, 5);

    Data::mesh_list.push_back(Mesh::create(vertices, indices));
}
void create_shaders_program() noexcept
{
    Data::shader_list.push_back(Shader::create_from_files(Data::vertex_shader_path, Data::fragment_shader_path));
}

int main()
{
    // Window dimensions
    constexpr GLint WIDTH = 800;
    constexpr GLint HEIGHT = 600;

    auto main_window = Window::create(WIDTH, HEIGHT, "Diffuse Lighting");

    if (main_window == nullptr)
    {
        return EXIT_FAILURE;
    }

    specify_vertices();
    create_shaders_program();

    Camera camera{glm::vec3{0.f, 0.f, 0.f}, glm::vec3{0.f, 1.f, 0.f}, 0.f, -90.f, 5.f, 20.0f};

    Texture brick_texture{Data::root_path / "textures" / "brick.png"};
    brick_texture.load();

    Light main_light{1.f, 1.f, 1.f, 0.2f, 2.0f, -1.f, -2.f, 1.f};

    glm::mat4 projection = glm::perspective(45.f, main_window->get_aspect_ratio(), 0.1f, 100.f);

    GLfloat last_time = glfwGetTime();
    
    while (!main_window->should_be_closed())
    {
        GLfloat now = glfwGetTime();
        GLfloat dt = now - last_time;
        last_time = now;

        // Get and handle user input events
        glfwPollEvents();

        camera.handle_keys(main_window->get_keys());
        camera.handle_mouse(main_window->get_x_change(), main_window->get_y_change());
        camera.update(dt);

        // Clear the window
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Data::shader_list[0]->use();

        glm::mat4 model{1.f};
        model = glm::translate(model, glm::vec3{0.f, 0.f, -2.5f});
        model = glm::scale(model, glm::vec3{0.4f, 0.4f, 1.f});

        glUniformMatrix4fv(Data::shader_list[0]->get_uniform_model_id(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(Data::shader_list[0]->get_uniform_projection_id(), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(Data::shader_list[0]->get_uniform_view_id(), 1, GL_FALSE, glm::value_ptr(camera.get_view_matrix()));
        main_light.use(Data::shader_list[0]->get_ambient_intensity_id(), Data::shader_list[0]->get_ambient_color_id(),
                       Data::shader_list[0]->get_uniform_diffuse_intensity_id(), Data::shader_list[0]->get_uniform_direction_diffuse_id());

        brick_texture.use();

        // Draw meshes
        for (const auto& mesh: Data::mesh_list)
        {
            mesh->render();
        }

        glUseProgram(0);

        main_window->swap_buffers();
    }
    
    return EXIT_SUCCESS;
}
