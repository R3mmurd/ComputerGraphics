#pragma once

#include <array>
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

    GLfloat get_x_change() noexcept;

    GLfloat get_y_change() noexcept;

    const std::array<bool, 1024>& get_keys() const noexcept { return keys; };

    bool should_be_closed() const noexcept;

    void swap_buffers() noexcept;

private:
    GLFWwindow* window{nullptr};
    GLint width{0};
    GLint height{0};
    GLint buffer_width{0};
    GLint buffer_height{0};

    GLfloat last_x{0.f};
    GLfloat last_y{0.f};
    GLfloat x_change{0.f};
    GLfloat y_change{0.f};
    bool mouse_first_move{true};

    std::array<bool, 1024> keys{};

    void create_callbacks() noexcept;

    static void handle_keys(GLFWwindow* window, int key, int code, int action, int mode) noexcept;

    static void handle_mouse(GLFWwindow* window, double x_pos, double y_pos) noexcept;
};
