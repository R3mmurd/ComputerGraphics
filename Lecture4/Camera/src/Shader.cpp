#include <fstream>

#include <Shader.hpp>

Shader::~Shader()
{
    clear();
}

std::shared_ptr<Shader> Shader::create_from_strings(std::string_view vertex_shader_code, std::string_view fragment_shader_code) noexcept
{
    auto shader = std::make_shared<Shader>();
    shader->create_program(vertex_shader_code, fragment_shader_code);
    return shader;
}

std::shared_ptr<Shader> Shader::create_from_files(std::filesystem::path vertex_shader_path, std::filesystem::path fragment_shader_path) noexcept
{
    std::string vertex_shader_code = read_file(vertex_shader_path);
    std::string fragment_shader_code = read_file(fragment_shader_path);
    return create_from_strings(vertex_shader_code, fragment_shader_code);
}

void Shader::use() const noexcept
{
    glUseProgram(program_id);
}

void Shader::create_program(std::string_view vertex_shader_code, std::string_view fragment_shader_code) noexcept
{
    LOG_INIT_CERR();

    program_id = glCreateProgram(); 

    if (!program_id)
    {
        log(LOG_ERR) << "Error creating shaders program\n";
        return;
    }

    create_shader(vertex_shader_code, GL_VERTEX_SHADER);
    create_shader(fragment_shader_code, GL_FRAGMENT_SHADER);

    glLinkProgram(program_id);

    GLint result;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);

    if (!result)
    {
        GLchar log_text[1024] = { 0 };
        glGetProgramInfoLog(program_id, sizeof(log_text), nullptr, log_text);
        log(LOG_ERR) << "Error linking the program: " << log_text << " \n";
        return;
    }

    glValidateProgram(program_id);
    glGetProgramiv(program_id, GL_VALIDATE_STATUS, &result);

    if (!result)
    {
        GLchar log_text[1024] = { 0 };
        glGetProgramInfoLog(program_id, sizeof(log_text), nullptr, log_text);
        log(LOG_ERR) << "Error validating the program: " << log_text << " \n";
    }

    uniform_model_id = glGetUniformLocation(program_id, "model");
    uniform_view_id = glGetUniformLocation(program_id, "view");
    uniform_projection_id = glGetUniformLocation(program_id, "projection");
}

void Shader::create_shader(std::string_view shader_code, GLenum shader_type) noexcept
{
    LOG_INIT_CERR();

    GLuint shader = glCreateShader(shader_type);

    const GLchar* code[1];
    code[0] = shader_code.data();

    GLint code_length[1];
    code_length[0] = shader_code.size();

    glShaderSource(shader, 1, code, code_length);
    glCompileShader(shader);

    GLint result;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    if (!result)
    {
        GLchar log_text[1024] = { 0 };
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log_text);
        log(LOG_ERR) << "Error compiling the shader " << shader_type << ": " << log_text << "\n";
        return;
    }

    glAttachShader(program_id, shader);
}

void Shader::clear() noexcept
{
    if (program_id != 0)
    {
        glDeleteProgram(program_id);
        program_id = 0;
    }

    uniform_projection_id = 0;
    uniform_model_id = 0;
}

std::string Shader::read_file(const std::filesystem::path& shader_path) noexcept
{
    LOG_INIT_CERR();

    std::string contents{""};

    std::ifstream in_stream{shader_path};

    if (!in_stream)
    {
        log(LOG_ERR) << "File " << shader_path << " not found\n";
        return "";
    }

    std::string line;
    while (std::getline(in_stream, line))
    {
        contents.append(line + "\n");
    }

    in_stream.close();

    return contents;
}