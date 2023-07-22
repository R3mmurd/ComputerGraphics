#include <fstream>
#include <sstream>

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

void Shader::set_directional_light(const DirectionalLight& light) const noexcept
{
    light.use(uniform_directional_light.uniform_color_id, uniform_directional_light.uniform_ambient_intensity_id, uniform_directional_light.uniform_diffuse_intensity_id, uniform_directional_light.uniform_direction_id);
}

void Shader::set_point_lights(const std::vector<PointLight>& lights) const noexcept
{
    size_t num_lights = std::min(MAX_POINT_LIGHTS, lights.size());
    glUniform1i(uniform_num_point_lights, num_lights);
    for (size_t i = 0; i < num_lights; ++i)
    {
        lights[i].use(uniform_point_lights[i].uniform_color_id, uniform_point_lights[i].uniform_ambient_intensity_id,
                      uniform_point_lights[i].uniform_diffuse_intensity_id, uniform_point_lights[i].uniform_position_id,
                      uniform_point_lights[i].uniform_a_id, uniform_point_lights[i].uniform_b_id, uniform_point_lights[i].uniform_c_id);
    }
}

void Shader::set_spot_lights(const std::vector<SpotLight>& lights) const noexcept
{
    size_t num_lights = std::min(MAX_SPOT_LIGHTS, lights.size());
    glUniform1i(uniform_num_spot_lights, num_lights);
    for (size_t i = 0; i < num_lights; ++i)
    {
        lights[i].use(uniform_spot_lights[i].uniform_color_id, uniform_spot_lights[i].uniform_ambient_intensity_id,
                      uniform_spot_lights[i].uniform_diffuse_intensity_id, uniform_spot_lights[i].uniform_position_id,
                      uniform_spot_lights[i].uniform_a_id, uniform_spot_lights[i].uniform_b_id, uniform_spot_lights[i].uniform_c_id,
                      uniform_spot_lights[i].uniform_direction_id, uniform_spot_lights[i].uniform_edge_id);
    }
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
    uniform_eye_position_id = glGetUniformLocation(program_id, "eye_position");
    uniform_directional_light.uniform_ambient_intensity_id = glGetUniformLocation(program_id, "directional_light.base.ambient_intensity");
    uniform_directional_light.uniform_color_id = glGetUniformLocation(program_id, "directional_light.base.color");
    uniform_directional_light.uniform_diffuse_intensity_id = glGetUniformLocation(program_id, "directional_light.base.diffuse_intensity");
    uniform_directional_light.uniform_direction_id = glGetUniformLocation(program_id, "directional_light.direction");
    uniform_specular_intensity_id = glGetUniformLocation(program_id, "material.specular_intensity");
    uniform_specular_shininess_id = glGetUniformLocation(program_id, "material.shininess");
    uniform_num_point_lights = glGetUniformLocation(program_id, "num_point_lights");
    uniform_num_spot_lights = glGetUniformLocation(program_id, "num_spot_lights");
    
    for (size_t i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        std::stringstream s1, s2, s3, s4, s5, s6, s7;
        
        s1 << "point_lights[" << i << "]" << ".base.color";
        uniform_point_lights[i].uniform_color_id = glGetUniformLocation(program_id, s1.str().c_str());
        
        s2 << "point_lights[" << i << "]" << ".base.ambient_intensity";
        uniform_point_lights[i].uniform_ambient_intensity_id = glGetUniformLocation(program_id, s2.str().c_str());
        
        s3 << "point_lights[" << i << "]" << ".base.diffuse_intensity";
        uniform_point_lights[i].uniform_diffuse_intensity_id = glGetUniformLocation(program_id, s3.str().c_str());
        
        s4 << "point_lights[" << i << "]" << ".position";
        uniform_point_lights[i].uniform_position_id = glGetUniformLocation(program_id, s4.str().c_str());
        
        s5 << "point_lights[" << i << "]" << ".a";
        uniform_point_lights[i].uniform_a_id = glGetUniformLocation(program_id, s5.str().c_str());
        
        s6 << "point_lights[" << i << "]" << ".b";
        uniform_point_lights[i].uniform_b_id = glGetUniformLocation(program_id, s6.str().c_str());
        
        s7 << "point_lights[" << i << "]" << ".c";
        uniform_point_lights[i].uniform_c_id = glGetUniformLocation(program_id, s7.str().c_str());
    }

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; ++i)
    {
        std::stringstream s1, s2, s3, s4, s5, s6, s7, s8, s9;
        
        s1 << "spot_lights[" << i << "]" << ".base.base.color";
        uniform_spot_lights[i].uniform_color_id = glGetUniformLocation(program_id, s1.str().c_str());
        
        s2 << "spot_lights[" << i << "]" << ".base.base.ambient_intensity";
        uniform_spot_lights[i].uniform_ambient_intensity_id = glGetUniformLocation(program_id, s2.str().c_str());
        
        s3 << "spot_lights[" << i << "]" << ".base.base.diffuse_intensity";
        uniform_spot_lights[i].uniform_diffuse_intensity_id = glGetUniformLocation(program_id, s3.str().c_str());
        
        s4 << "spot_lights[" << i << "]" << ".base.position";
        uniform_spot_lights[i].uniform_position_id = glGetUniformLocation(program_id, s4.str().c_str());
        
        s5 << "spot_lights[" << i << "]" << ".base.a";
        uniform_spot_lights[i].uniform_a_id = glGetUniformLocation(program_id, s5.str().c_str());
        
        s6 << "spot_lights[" << i << "]" << ".base.b";
        uniform_spot_lights[i].uniform_b_id = glGetUniformLocation(program_id, s6.str().c_str());
        
        s7 << "spot_lights[" << i << "]" << ".base.c";
        uniform_spot_lights[i].uniform_c_id = glGetUniformLocation(program_id, s7.str().c_str());

        s8 << "spot_lights[" << i << "]" << ".direction";
        uniform_spot_lights[i].uniform_direction_id = glGetUniformLocation(program_id, s8.str().c_str());

        s9 << "spot_lights[" << i << "]" << ".edge";
        uniform_spot_lights[i].uniform_edge_id = glGetUniformLocation(program_id, s9.str().c_str());
    }
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
    uniform_view_id = 0;
    uniform_model_id = 0;
    uniform_eye_position_id = 0;
    uniform_directional_light.uniform_ambient_intensity_id = 0;
    uniform_directional_light.uniform_color_id = 0; 
    uniform_directional_light.uniform_diffuse_intensity_id = 0;
    uniform_directional_light.uniform_direction_id = 0;
    uniform_specular_intensity_id = 0;
    uniform_specular_shininess_id = 0;
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