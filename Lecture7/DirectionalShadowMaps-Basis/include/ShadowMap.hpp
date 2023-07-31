#pragma once

#include <GL/glew.h>

#include <BSlogger.hpp>

class ShadowMap
{
public:
    ShadowMap() = default;

    virtual ~ShadowMap();

    virtual bool init(GLuint w, GLuint h) noexcept;

    virtual void write() const noexcept;

    virtual void read(GLenum texture_unit) noexcept;

    GLuint get_width() const noexcept { return width; }

    GLuint get_height() const noexcept { return height; }

private:
    GLuint FBO_id{0};
    GLuint shadow_map_id{0};
    GLuint width{0};
    GLuint height{0};
};
