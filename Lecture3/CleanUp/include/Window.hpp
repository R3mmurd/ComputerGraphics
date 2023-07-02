#pragma once

#include <memory>
#include <string_view>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <BSlogger.hpp>

class Window
{
public:
    Window() = default;

    Window(const Window& window) = delete;

    Window(Window&& window) = delete;

    ~Window();

    Window& operator = (const Window& window) = delete;

    Window& operator = (Window&& window) = delete;

    static std::shared_ptr<Window> create(GLint width, GLint height, std::string_view title) noexcept;

    GLint get_buffer_width() const noexcept { return buffer_width; }

    GLint get_buffer_height() const noexcept { return buffer_height; }

    GLfloat get_aspect_ratio() const noexcept;

    bool should_be_closed() const noexcept;

    void swap_buffers() noexcept;

private:
    GLFWwindow* window{nullptr};
    GLint width{0};
    GLint height{0};
    GLint buffer_width{0};
    GLint buffer_height{0};
};
