#include <Window.hpp>

Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


std::shared_ptr<Window> Window::create(GLint width, GLint height, std::string_view title) noexcept
{
    LOG_INIT_CERR();

    auto window = std::make_shared<Window>();

    window->width = width;
    window->height = height;

    if (!glfwInit())
    {
        log(LOG_ERR) << "GLFW initialization failed!\n";
        glfwTerminate();
        return nullptr;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // No backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window->window = glfwCreateWindow(width, height, title.data(), nullptr, nullptr);

    if (!window->window)
    {
        log(LOG_ERR) << "GLFW window creation failed!\n";
        glfwTerminate();
        return nullptr;
    }
    
    // Set context for GLEW
    glfwMakeContextCurrent(window->window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        log(LOG_ERR) << "Glew intialization failed!\n";
        glfwDestroyWindow(window->window);
        glfwTerminate();
        return nullptr;
    }

    // Get buffer size
    glfwGetFramebufferSize(window->window, &window->buffer_width, &window->buffer_height);

    glEnable(GL_DEPTH_TEST);

    // Setup viewport
    glViewport(0, 0, window->buffer_width, window->buffer_height);

    return window;
}

GLfloat Window::get_aspect_ratio() const noexcept
{
    return GLfloat(get_buffer_width()) / GLfloat(get_buffer_height());
}

bool Window::should_be_closed() const noexcept
{
    return glfwWindowShouldClose(window);
}

void Window::swap_buffers() noexcept
{
    glfwSwapBuffers(window);
}
