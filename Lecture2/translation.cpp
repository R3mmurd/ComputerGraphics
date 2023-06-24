#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Data
{
    static GLuint VAO_id;
    static GLuint VBO_id;
    static GLuint shaders_id;
    static GLuint uniform_translation_matrix_id;

    static const std::string vertex_shader;
    static const std::string fragment_shader;

    static constexpr float triangle_max_offset{0.7f};
    static constexpr float translation_increment{0.005f};
};

GLuint Data::VAO_id{};
GLuint Data::VBO_id{};
GLuint Data::shaders_id{};
GLuint Data::uniform_translation_matrix_id{};

const std::string Data::vertex_shader = R"(
#version 410

layout(location = 0) in vec3 pos;

uniform mat4 translation_matrix;

void main()
{
    gl_Position = translation_matrix * vec4(0.3 * pos.x, 0.3 * pos.y, pos.z, 1.0);
}
)";

const std::string Data::fragment_shader = R"(
#version 410

out vec4 color;

void main()
{
    color = vec4(0.0, 1.0, 0.0, 1.0);
}
)";

void specify_vertices()
{
    GLfloat vertices[] = {
        -1.f, -1.f, 0.f,
        1.f, -1.f, 0.f,
        0.f, 1.f, 0.f
    };

    glGenVertexArrays(1, &Data::VAO_id);
    glBindVertexArray(Data::VAO_id);

    glGenBuffers(1, &Data::VBO_id);
    glBindBuffer(GL_ARRAY_BUFFER, Data::VBO_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

void create_shader(GLuint program, const std::string& shader_code, GLenum shader_type)
{
    GLuint shader = glCreateShader(shader_type);

    const GLchar* code[1];
    code[0] = shader_code.c_str();

    GLint code_length[1];
    code_length[0] = shader_code.size();

    glShaderSource(shader, 1, code, code_length);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        GLchar log[1024] = { 0 };
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        std::cerr << "Error compiling the shader " << shader_type << ": " << log << std::endl;
        return;
    }

    glAttachShader(program, shader);
}

void create_shaders_program()
{
    Data::shaders_id = glCreateProgram(); 

    if (!Data::shaders_id)
    {
        std::cerr << "Error creating shaders program\n";
        return;
    }

    create_shader(Data::shaders_id, Data::vertex_shader, GL_VERTEX_SHADER);
    create_shader(Data::shaders_id, Data::fragment_shader, GL_FRAGMENT_SHADER);

    glLinkProgram(Data::shaders_id);

    GLint result;
    glGetProgramiv(Data::shaders_id, GL_LINK_STATUS, &result);

    if (!result)
    {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(Data::shaders_id, sizeof(log), nullptr, log);
        std::cerr << "Error linking the program: " << log << std::endl;
        return;
    }

    glValidateProgram(Data::shaders_id);
    glGetProgramiv(Data::shaders_id, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        GLchar log[1024] = { 0 };
        glGetProgramInfoLog(Data::shaders_id, sizeof(log), nullptr, log);
        std::cerr << "Error validating the program: " << log << std::endl;
    }

    Data::uniform_translation_matrix_id = glGetUniformLocation(Data::shaders_id, "translation_matrix");
}

int main()
{
    // Window dimensions
    constexpr GLint WIDTH = 800;
    constexpr GLint HEIGHT = 600;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // No backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* main_window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", nullptr, nullptr);

    if (!main_window)
    {
        std::cerr << "GLFW window creation failed!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    // Set context for GLEW
    glfwMakeContextCurrent(main_window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Glew intialization failed!\n";
        glfwDestroyWindow(main_window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Get buffer size
    int buffer_width;
    int buffer_height;
    glfwGetFramebufferSize(main_window, &buffer_width, &buffer_height);

    // Setup viewport
    glViewport(0, 0, buffer_width, buffer_height);

    specify_vertices();
    create_shaders_program();

    bool moving_to_right{true};
    float current_triangle_offset{0.f};
    
    while (!glfwWindowShouldClose(main_window))
    {
        // Get and handle user input events
        glfwPollEvents();

        if (moving_to_right)
        {
            current_triangle_offset += Data::translation_increment;
        }
        else
        {
            current_triangle_offset -= Data::translation_increment;
        }

        if (std::abs(current_triangle_offset) >= Data::triangle_max_offset)
        {
            moving_to_right = !moving_to_right;
        }

        // Clear the window
        glClearColor(0.f, 0.f, 0.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(Data::shaders_id);

        glm::mat4 translation_matrix{1.f};
        translation_matrix = glm::translate(translation_matrix, glm::vec3{current_triangle_offset, 0.f, 0.f});

        glUniformMatrix4fv(Data::uniform_translation_matrix_id, 1, GL_FALSE, glm::value_ptr(translation_matrix));

        glBindVertexArray(Data::VAO_id);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glUseProgram(0);

        glfwSwapBuffers(main_window);
    }
    
    return EXIT_SUCCESS;
}
