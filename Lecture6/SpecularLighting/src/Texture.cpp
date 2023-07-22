#define STB_IMAGE_IMPLEMENTATION

#include <Texture.hpp>

Texture::Texture(const std::filesystem::path& _file_path)
    : file_path{_file_path}
{

}

Texture::~Texture()
{
    clear();
}

void Texture::load() noexcept
{
    unsigned char* tex_data = stbi_load(file_path.c_str(), &width, &height, &bit_depth, 0);

    if (!tex_data)
    {
        LOG_INIT_CERR();
        log(LOG_ERR) << "Failed to find: " << file_path << "\n";
        return;
    }

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(tex_data);
}

void Texture::use() const noexcept
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::clear() noexcept
{
    glDeleteTextures(1, &id);
    id = 0;
    width = 0;
    height = 0;
    bit_depth = 0;
    file_path.clear();
}