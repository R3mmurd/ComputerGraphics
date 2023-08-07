#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Camera.hpp>
#include <DirectionalLight.hpp>
#include <Material.hpp>
#include <Mesh.hpp>
#include <Model.hpp>
#include <PointLight.hpp>
#include <Shader.hpp>
#include <SkyBox.hpp>
#include <SpotLight.hpp>
#include <Texture.hpp>
#include <Window.hpp>

namespace fs = std::filesystem;

struct Data
{
    static constexpr GLint WIDTH = 1024;
    static constexpr GLint HEIGHT = 768;
    static std::shared_ptr<SkyBox> sky_box;
    static std::vector<std::shared_ptr<Shader>> shader_list;
    static std::vector<std::shared_ptr<Mesh>> mesh_list;
    static std::vector<std::shared_ptr<Texture>> texture_list;
    static std::vector<std::shared_ptr<Material>> material_list;
    static std::vector<std::shared_ptr<Model>> model_list;
    static std::shared_ptr<Camera> camera;
    static std::shared_ptr<DirectionalLight> main_light;
    static std::vector<std::shared_ptr<PointLight>> point_lights;
    static std::vector<std::shared_ptr<SpotLight>> spot_lights;
    static const fs::path root_path;
    static const fs::path vertex_shader_path;
    static const fs::path fragment_shader_path;
    static const fs::path directional_shadow_map_vertex_shader_path;
    static const fs::path directional_shadow_map_fragment_shader_path;
    static const fs::path omnidirectional_shadow_map_vertex_shader_path;
    static const fs::path omnidirectional_shadow_map_geometry_shader_path;
    static const fs::path omnidirectional_shadow_map_fragment_shader_path;

    static float black_hawk_angle;

    // Shader variable locations
    static GLuint uniform_projection_id;
    static GLuint uniform_model_id;
    static GLuint uniform_view_id;
    static GLuint uniform_eye_position_id;
    static GLuint uniform_specular_intensity_id;
    static GLuint uniform_shininess_id;
    static GLuint uniform_directional_light_space_transform_id;
    static GLuint uniform_omnidirectional_light_position_id;
    static GLuint uniform_far_plane_id;
};

std::shared_ptr<SkyBox> Data::sky_box{nullptr};
std::vector<std::shared_ptr<Shader>> Data::shader_list{};
std::vector<std::shared_ptr<Mesh>> Data::mesh_list{};
std::vector<std::shared_ptr<Texture>> Data::texture_list{};
std::vector<std::shared_ptr<Material>> Data::material_list{};
std::vector<std::shared_ptr<Model>> Data::model_list{};
std::shared_ptr<Camera> Data::camera{nullptr};
std::shared_ptr<DirectionalLight> Data::main_light{nullptr};
std::vector<std::shared_ptr<PointLight>> Data::point_lights{};
std::vector<std::shared_ptr<SpotLight>> Data::spot_lights{};

const fs::path Data::root_path{fs::path{__FILE__}.parent_path()};
const fs::path Data::vertex_shader_path{Data::root_path / "shaders" / "shader.vert"};
const fs::path Data::fragment_shader_path{Data::root_path / "shaders" / "shader.frag"};
const fs::path Data::directional_shadow_map_vertex_shader_path{Data::root_path / "shaders" / "directional_shadow_map.vert"};
const fs::path Data::directional_shadow_map_fragment_shader_path{Data::root_path / "shaders" / "directional_shadow_map.frag"};
const fs::path Data::omnidirectional_shadow_map_vertex_shader_path{Data::root_path / "shaders" / "omnidirectional_shadow_map.vert"};
const fs::path Data::omnidirectional_shadow_map_geometry_shader_path{Data::root_path / "shaders" / "omnidirectional_shadow_map.geom"};
const fs::path Data::omnidirectional_shadow_map_fragment_shader_path{Data::root_path / "shaders" / "omnidirectional_shadow_map.frag"};

float Data::black_hawk_angle{0.f};

GLuint Data::uniform_projection_id{0};
GLuint Data::uniform_model_id{0};
GLuint Data::uniform_view_id{0};
GLuint Data::uniform_eye_position_id{0};
GLuint Data::uniform_specular_intensity_id{0};
GLuint Data::uniform_shininess_id{0};
GLuint Data::uniform_directional_light_space_transform_id{0};
GLuint Data::uniform_omnidirectional_light_position_id{0};
GLuint Data::uniform_far_plane_id{0};

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

    std::vector<unsigned int> floor_indices{
        0, 2, 1,
        1, 2, 3
    };

    std::vector<GLfloat> floor_vertices{
    //    x     y    z      u      v     nx   ny   nz
        -10.f, 0.f, -10.f,  0.f,  0.f,  0.f, -1.f, 0.f,
         10.f, 0.f, -10.f, 10.f,  0.f,  0.f, -1.f, 0.f,
        -10.f, -0.f, 10.f,  0.f, 10.f,  0.f, -1.f, 0.f,
         10.f, 0.f,  10.f, 10.f, 10.f,  0.f, -1.f, 0.f
    };

    Data::mesh_list.push_back(Mesh::create(vertices, indices));
    Data::mesh_list.push_back(Mesh::create(vertices, indices));
    Data::mesh_list.push_back(Mesh::create(floor_vertices, floor_indices));
}
void create_shaders_program() noexcept
{
    Data::shader_list.push_back(Shader::create_from_files(Data::vertex_shader_path, Data::fragment_shader_path));
    Data::shader_list.push_back(Shader::create_from_files(Data::directional_shadow_map_vertex_shader_path, Data::directional_shadow_map_fragment_shader_path));
    Data::shader_list.push_back(Shader::create_from_files(Data::omnidirectional_shadow_map_vertex_shader_path, Data::omnidirectional_shadow_map_geometry_shader_path, Data::omnidirectional_shadow_map_fragment_shader_path));
}

void create_textures_and_materials() noexcept
{
    auto brick_texture = std::make_shared<Texture>(Data::root_path / "textures" / "brick.png");
    brick_texture->load_a();
    Data::texture_list.push_back(brick_texture);

    auto dirt_texture = std::make_shared<Texture>(Data::root_path / "textures" / "dirt.png");
    dirt_texture->load_a();
    Data::texture_list.push_back(dirt_texture);

    Data::material_list.push_back(std::make_shared<Material>(1.f, 32.f)); // Shiny
    Data::material_list.push_back(std::make_shared<Material>(0.3f, 4.f)); // Dull
}

void render_scene() noexcept
{
    glm::mat4 model{1.f};
    model = glm::translate(model, glm::vec3{0.f, 2.f, -2.5f});
    glUniformMatrix4fv(Data::uniform_model_id, 1, GL_FALSE, glm::value_ptr(model));
    Data::texture_list[0]->use();
    Data::material_list[0]->use(Data::uniform_specular_intensity_id, Data::uniform_shininess_id);
    Data::mesh_list[0]->render();

    model = glm::mat4{1.f};
    model = glm::translate(model, glm::vec3{0.f, 4.f, -2.5f});
    glUniformMatrix4fv(Data::uniform_model_id, 1, GL_FALSE, glm::value_ptr(model));
    Data::texture_list[1]->use();
    Data::material_list[1]->use(Data::uniform_specular_intensity_id, Data::uniform_shininess_id);
    Data::mesh_list[1]->render();

    model = glm::mat4{1.f};
    model = glm::translate(model, glm::vec3{0.f, -2.f, 0.f});
    glUniformMatrix4fv(Data::uniform_model_id, 1, GL_FALSE, glm::value_ptr(model));
    Data::texture_list[1]->use();
    Data::material_list[1]->use(Data::uniform_specular_intensity_id, Data::uniform_shininess_id);
    Data::mesh_list[2]->render();

    model = glm::mat4{1.f};
    model = glm::translate(model, glm::vec3{-20.f, 0.f, 15.f});
    model = glm::scale(model, glm::vec3{0.01f, 0.01f, 0.01f});
    glUniformMatrix4fv(Data::uniform_model_id, 1, GL_FALSE, glm::value_ptr(model));
    Data::material_list[0]->use(Data::uniform_specular_intensity_id, Data::uniform_shininess_id);
    Data::model_list[0]->render();

    Data::black_hawk_angle += 0.1f;

	if (Data::black_hawk_angle > 360.0f)
	{
		Data::black_hawk_angle = 0.1f;
	}

    model = glm::mat4(1.0f);
	model = glm::rotate(model, to_radian(-Data::black_hawk_angle), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-8.f, 2.f, 0.f));
	model = glm::rotate(model, to_radian(-20.f), glm::vec3(0.f, 0.f, 1.f));
	model = glm::rotate(model, to_radian(-90.f), glm::vec3(1.f, 0.f, 0.f));
	model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
	glUniformMatrix4fv(Data::uniform_model_id, 1, GL_FALSE, glm::value_ptr(model));
	Data::material_list[0]->use(Data::uniform_specular_intensity_id, Data::uniform_shininess_id);
    Data::model_list[1]->render();
}

void directional_shadow_map_pass(std::shared_ptr<DirectionalLight> light) noexcept
{
    Data::shader_list[1]->use();
    
    glViewport(0, 0, light->get_shadow_map()->get_width(), light->get_shadow_map()->get_height());
    
    light->get_shadow_map()->write();

    glClear(GL_DEPTH_BUFFER_BIT);

    Data::uniform_model_id = Data::shader_list[1]->get_uniform_model_id();
    Data::shader_list[1]->set_directional_light_space_transform(light->get_light_transform());

    render_scene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void omnidirectional_shadow_map_pass(std::shared_ptr<PointLight> light) noexcept
{
    glViewport(0, 0, light->get_shadow_map()->get_width(), light->get_shadow_map()->get_height());

    Data::shader_list[2]->use();

    Data::uniform_model_id = Data::shader_list[2]->get_uniform_model_id();
    Data::uniform_omnidirectional_light_position_id = Data::shader_list[2]->get_uniform_omnidirectional_light_position_id();
    Data::uniform_far_plane_id = Data::shader_list[2]->get_uniform_far_plane_id();

    light->get_shadow_map()->write();

    glClear(GL_DEPTH_BUFFER_BIT);

    glUniform3f(Data::uniform_omnidirectional_light_position_id, light->get_position().x, light->get_position().y, light->get_position().z);
    glUniform1f(Data::uniform_far_plane_id, light->get_far_plane());
    Data::shader_list[2]->set_omnidirectional_light_matrices(light->get_light_transforms());
    
    render_scene();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void render_pass(const glm::mat4& projection, const glm::mat4& view) noexcept
{
    glViewport(0, 0, Data::WIDTH, Data::HEIGHT);

    // Clear the window
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Data::sky_box->render(view, projection);

    Data::shader_list[0]->use();

    Data::uniform_model_id = Data::shader_list[0]->get_uniform_model_id();
    Data::uniform_projection_id = Data::shader_list[0]->get_uniform_projection_id();
    Data::uniform_view_id = Data::shader_list[0]->get_uniform_view_id();
    Data::uniform_eye_position_id = Data::shader_list[0]->get_uniform_eye_position_id();
    Data::uniform_specular_intensity_id = Data::shader_list[0]->get_uniform_specular_intensity_id();
    Data::uniform_shininess_id = Data::shader_list[0]->get_uniform_specular_shininess_id();

    glUniformMatrix4fv(Data::shader_list[0]->get_uniform_projection_id(), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(Data::shader_list[0]->get_uniform_view_id(), 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(Data::shader_list[0]->get_uniform_eye_position_id(), Data::camera->get_position().x, Data::camera->get_position().y, Data::camera->get_position().z);

    Data::shader_list[0]->set_directional_light(Data::main_light);
    Data::shader_list[0]->set_point_lights(Data::point_lights, 3, 0);
    Data::shader_list[0]->set_spot_lights(Data::spot_lights, 3 + Data::point_lights.size(), Data::point_lights.size());
    Data::shader_list[0]->set_directional_light_space_transform(Data::main_light->get_light_transform());

    Data::main_light->get_shadow_map()->read(GL_TEXTURE2);
    Data::shader_list[0]->set_texture(1);
    Data::shader_list[0]->set_directional_shadow_map(2);

    auto lower_light = Data::camera->get_position();
    lower_light.y -= 0.3f;
    //Data::spot_lights[0]->set(lower_light, Data::camera->get_direction());

    render_scene();
}

int main()
{

    auto main_window = Window::create(Data::WIDTH, Data::HEIGHT, "Skybox");

    if (main_window == nullptr)
    {
        return EXIT_FAILURE;
    }

    specify_vertices();
    create_shaders_program();
    create_textures_and_materials();

    auto xwing = std::make_shared<Model>(Data::root_path);
    xwing->load("x-wing.obj");
    Data::model_list.push_back(xwing);

    auto black_hawk = std::make_shared<Model>(Data::root_path);
    black_hawk->load("uh60.obj");
    Data::model_list.push_back(black_hawk);

    Data::camera = std::make_shared<Camera>(glm::vec3{-3.f, 2.f, 3.f}, glm::vec3{0.f, 1.f, 0.f}, 0.f, -60.f, 5.f, 20.0f);

    Data::main_light = std::make_shared<DirectionalLight>(
        1024, 1024,         // shadow map size
        1.f, 0.5f, 0.3f,    // color
        0.1f, 0.6f,         // ambient and diffuse intensity
        -8.f, -10.f, 14.f   //direction
    );

    /*Data::point_lights.push_back(
        std::make_shared<PointLight>(
            1024, 1024,        // shadow map size
            0.1f, 100.f,       // near and far
            0.f, 0.f, 1.f,     // color
            0.f, 0.4f,         // ambient and diffuse intensity
            4.f, 0.f, 0.f,     // position
            0.01f, 0.01f, 0.3f // a, b, c
        )
    );
    Data::point_lights.push_back(
        std::make_shared<PointLight>(
            1024, 1024,        // shadow map size
            0.1f, 100.f,       // near and far
            0.f, 1.f, 0.f,     // color
            0.f, 0.4f,         // ambient and diffuse intensity
            -4.f, 2.f, 0.f,    // position
            0.01f, 0.01f, 0.3f // a, b, c
        )
    );*/

    /*Data::spot_lights.push_back(
        std::make_shared<SpotLight>(
            1024, 1024,      // shadow map size
            0.1f, 100.f,     // near and far
            1.f, 1.f, 1.f,   // color
            0.f, 2.f,        // ambient and diffuse intensity
            0.f, 3.f, -5.f,  // position
            0.f, 0.f, 1.f,   // a, b, c
            0.f, -1.f, 0.f,  // direction
            20.f             // edge
        )
    );*/

    Data::sky_box = std::make_shared<SkyBox>(
        Data::root_path,
        std::vector<fs::path>{{
            "cupertin-lake_rt.tga",
            "cupertin-lake_lf.tga",
            "cupertin-lake_up.tga",
            "cupertin-lake_dn.tga",
            "cupertin-lake_bk.tga",
            "cupertin-lake_ft.tga",
        }}
    );

    glm::mat4 projection = glm::perspective(glm::radians(60.f), main_window->get_aspect_ratio(), 0.1f, 100.f);

    GLfloat last_time = glfwGetTime();
    
    while (!main_window->should_be_closed())
    {
        GLfloat now = glfwGetTime();
        GLfloat dt = now - last_time;
        last_time = now;

        // Get and handle user input events
        glfwPollEvents();

        Data::camera->handle_keys(main_window->get_keys());
        Data::camera->handle_mouse(main_window->get_x_change(), main_window->get_y_change());
        Data::camera->update(dt);

        directional_shadow_map_pass(Data::main_light);  

        for (auto light: Data::point_lights)
        {
            omnidirectional_shadow_map_pass(light);
        }

        for (auto light: Data::spot_lights)
        {
            omnidirectional_shadow_map_pass(light);
        }

        render_pass(projection, Data::camera->get_view_matrix());

        glUseProgram(0);

        main_window->swap_buffers();
    }
    
    return EXIT_SUCCESS;
}
