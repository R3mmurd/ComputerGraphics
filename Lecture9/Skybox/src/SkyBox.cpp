#include <SkyBox.hpp>

const std::filesystem::path& SkyBox::vertex_shader_filename{"skybox.vert"};
const std::filesystem::path& SkyBox::fragment_shader_filename{"skybox.frag"};

SkyBox::SkyBox(const std::filesystem::path& root_path, const std::vector<std::filesystem::path>& face_filenames) noexcept
{
    // Shader setup
    shader = Shader::create_from_files(root_path / "shaders" / vertex_shader_filename, root_path / "shaders" / fragment_shader_filename);
    uniform_projection_id = shader->get_uniform_projection_id();
    uniform_view_id = shader->get_uniform_view_id();

    // Texture setup
    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    int width{0};
    int height{0};
    int bit_depth{0};

    for (size_t i = 0; i < face_filenames.size(); ++i)
    {
        auto file_path = root_path / "textures" / "skybox" / face_filenames[i];
        unsigned char* tex_data = stbi_load(file_path.c_str(), &width, &height, &bit_depth, 0);

        if (!tex_data)
        {
            LOG_INIT_CERR();
            log(LOG_ERR) << "Failed to find: " << file_path << "\n";
            return;
        }

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_data);

        stbi_image_free(tex_data);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Mesh setup
    std::vector<unsigned int> indices{{
        // front
        0, 1, 2,
        2, 1, 3,
        // right
        2, 3, 5,
        5, 3, 7,
        // back
        5, 7, 4,
        4, 7, 6,
        // left
        4, 6, 0,
        0, 6, 1,
        // top
        4, 0, 5,
        5, 0, 2,
        // bottom
        1, 6, 3,
        3, 6, 7
    }};

    std::vector<float> vertices{{
        -1.f, 1.f, -1.f,	0.f, 0.f,		0.f, 0.f, 0.f,
        -1.f, -1.f, -1.f,	0.f, 0.f,		0.f, 0.f, 0.f,
        1.f, 1.f, -1.f,		0.f, 0.f,		0.f, 0.f, 0.f,
        1.f, -1.f, -1.f,	0.f, 0.f,		0.f, 0.f, 0.f,

        -1.f, 1.f, 1.f,		0.f, 0.f,		0.f, 0.f, 0.f,
        1.f, 1.f, 1.f,		0.f, 0.f,		0.f, 0.f, 0.f,
        -1.f, -1.f, 1.f,	0.f, 0.f,		0.f, 0.f, 0.f,
        1.f, -1.f, 1.f,		0.f, 0.f,		0.f, 0.f, 0.f
    }};

    mesh = Mesh::create(vertices, indices);
}

SkyBox::~SkyBox()
{
    if (texture_id)
    {
        glDeleteTextures(1, &texture_id);
        texture_id = 0;
    }
}

void SkyBox::render(const glm::mat4& view, const glm::mat4 projection) const noexcept
{
    // Removing translations
    glm::mat4 the_view = glm::mat4(glm::mat3(view));

    glDepthMask(GL_FALSE);

    shader->use();

    glUniformMatrix4fv(shader->get_uniform_view_id(), 1, GL_FALSE, glm::value_ptr(the_view));
    glUniformMatrix4fv(shader->get_uniform_projection_id(), 1, GL_FALSE, glm::value_ptr(projection));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture_id);

    mesh->render();

    glDepthMask(GL_TRUE);
}