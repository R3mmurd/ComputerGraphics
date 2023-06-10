#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    // Window dimensions
    constexpr GLint WIDTH = 800;
    constexpr GLint HEIGHT = 600;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "GLFW initialization failed!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Setup GLFW window properties
    // OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // No backward compatibility
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Allow forward compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* main_window = glfwCreateWindow(WIDTH, HEIGHT, "Hello World", NULL, NULL);

    if (!main_window)
    {
        std::cerr << "GLFW window creation failed!\n";
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    // Set context for GLEW
    glfwMakeContextCurrent(main_window);

    // Allow modern extension features
    glewExperimental = GL_TRUE;

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Glew intialization failed!\n";
        glfwDestroyWindow(main_window);
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Get buffer size
    int buffer_width;
    int buffer_height;
    glfwGetFramebufferSize(main_window, &buffer_width, &buffer_height);

    // Setup viewport
    glViewport(0, 0, buffer_width, buffer_height);
    
    while (!glfwWindowShouldClose(main_window))
    {
        // Get and handle user input events
        glfwPollEvents();

        // Clear the window
        glClearColor(0.f, 0.f, 1.f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(main_window);
    }
    
    return EXIT_SUCCESS;
}

