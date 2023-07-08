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

    glfwSetWindowUserPointer(window->window, window.get());

    window->create_callbacks();

    return window;
}

GLfloat Window::get_aspect_ratio() const noexcept
{
    return GLfloat(get_buffer_width()) / GLfloat(get_buffer_height());
}

GLfloat Window::get_x_change() noexcept
{
    GLfloat current_x_change = x_change;
    x_change = 0.f;
    return current_x_change;
}

GLfloat Window::get_y_change() noexcept
{
    GLfloat current_y_change = y_change;
    y_change = 0.f;
    return current_y_change;
}

bool Window::should_be_closed() const noexcept
{
    return glfwWindowShouldClose(window);
}

void Window::swap_buffers() noexcept
{
    glfwSwapBuffers(window);
}

void Window::create_callbacks() noexcept
{
    glfwSetKeyCallback(window, Window::handle_keys);
    glfwSetCursorPosCallback(window, Window::handle_mouse);
}

void Window::handle_keys(GLFWwindow* window, int key, int, int action, int) noexcept
{
    LOG_INIT_COUT();

    Window* window_obj = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key > 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            window_obj->keys[key] = true;
            log(LOG_INFO) << "Pressed key: " << key << "\n";
        }
        else if (action == GLFW_RELEASE)
        {
            window_obj->keys[key] = false;
            log(LOG_INFO) << "Released key: " << key << "\n";
        }
    }
}

void Window::handle_mouse(GLFWwindow* window, double x_pos, double y_pos) noexcept
{
    LOG_INIT_COUT();

    Window* window_obj = static_cast<Window*>(glfwGetWindowUserPointer(window));

    if (window_obj->mouse_first_move)
    {
        window_obj->last_x = x_pos;
        window_obj->last_y = y_pos;
        window_obj->mouse_first_move = false;
    }
    
    window_obj->x_change = x_pos - window_obj->last_x;
    window_obj->y_change = y_pos - window_obj->last_y;
    window_obj->last_x = x_pos;
    window_obj->last_y = y_pos;

    log(LOG_INFO) << "x: " << window_obj->x_change << " y: " << window_obj->y_change<< "\n";
}