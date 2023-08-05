#pragma once

#include <filesystem>

#include <GL/glew.h>

#include <stb_image.h>

#include <BSlogger.hpp>

class Texture
{
public:
    Texture() = default;

    Texture(const std::filesystem::path& _file_path);

    ~Texture();

    bool load() noexcept;

    bool load_a() noexcept;

    void use() const noexcept;

private:
    bool load_by_pixel_format(unsigned long pixel_format) noexcept;

    void clear() noexcept;

    GLuint id{0};
    int width{0};
    int height{0};
    int bit_depth{0};
    std::filesystem::path file_path;
};