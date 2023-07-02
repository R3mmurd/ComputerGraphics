#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Mesh.hpp>
#include <Shader.hpp>
#include <Window.hpp>

namespace fs = std::filesystem;

struct Data
{
    static std::vector<std::shared_ptr<Mesh>> mesh_list;
    static std::vector<std::shared_ptr<Shader>> shader_list;
    static const fs::path shaders_path;
    static const fs::path vertex_shader_path;
    static const fs::path fragment_shader_path;
};

std::vector<std::shared_ptr<Mesh>> Data::mesh_list{};
std::vector<std::shared_ptr<Shader>> Data::shader_list{};

const fs::path Data::shaders_path{fs::path{__FILE__}.parent_path() / "shaders"};
const fs::path Data::vertex_shader_path{Data::shaders_path / "shader.vert"};
const fs::path Data::fragment_shader_path{Data::shaders_path / "shader.frag"};

float to_radian(float degrees)
{
    return degrees * M_PI / 180.f;
}

void specify_vertices()
{
    std::vector<unsigned int> indices{
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };

    std::vector<GLfloat> vertices{
        -1.f, -1.f, 0.f,
        0.f, -1., 1.f,
        1.f, -1.f, 0.f,
        0.f, 1.f, 0.f
    };

    Data::mesh_list.push_back(Mesh::create(vertices, indices));
}
void create_shaders_program()
{
    Data::shader_list.push_back(Shader::create_from_files(Data::vertex_shader_path, Data::fragment_shader_path));
}

int main()
{
    // Window dimensions
    constexpr GLint WIDTH = 800;
    constexpr GLint HEIGHT = 600;

    auto main_window = Window::create(WIDTH, HEIGHT, "Clean Up");

    if (main_window == nullptr)
    {
        return EXIT_FAILURE;
    }

    specify_vertices();
    create_shaders_program();

    float current_angle{0.f};

    glm::mat4 projection = glm::perspective(45.f, main_window->get_aspect_ratio(), 0.1f, 100.f);
    
    while (!main_window->should_be_closed())
    {
        // Get and handle user input events
        glfwPollEvents();

        current_angle += 0.5f;

        if (current_angle >= 360.f)
        {
            current_angle = 0.f;
        }

        // Clear the window
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Data::shader_list[0]->use();

        glm::mat4 model{1.f};
        model = glm::translate(model,glm::vec3{0.f, 0.f, -2.5f});
        model = glm::rotate(model, to_radian(current_angle), glm::vec3{0.f, 1.f, 0.f});
        model = glm::scale(model, glm::vec3{0.4f, 0.4f, 1.f});

        glUniformMatrix4fv(Data::shader_list[0]->get_model_id(), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(Data::shader_list[0]->get_projection_id(), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw meshes
        for (const auto& mesh: Data::mesh_list)
        {
            mesh->render();
        }

        main_window->swap_buffers();
    }
    
    return EXIT_SUCCESS;
}
