#pragma once

#include <array>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
public:
    Camera(const glm::vec3& _position, const glm::vec3& _world_up, GLfloat _pitch, GLfloat _yaw, GLfloat _movement_speed, GLfloat _rotation_speed) noexcept;

    Camera(const Camera& camera) = delete;

    Camera(Camera&& camera) = delete;

    ~Camera();

    Camera operator = (const Camera& camera) = delete;

    Camera operator = (Camera&& camera) = delete;

    void handle_keys(const std::array<bool, 1024>& keys) noexcept;

    void handle_mouse(GLfloat x_change, GLfloat y_change) noexcept;

    void update(GLfloat dt) noexcept;

    glm::mat4 get_view_matrix() const noexcept;

private:
    void update_vectors() noexcept;

    glm::vec3 position;
    glm::vec3 front{0.f, 0.f, -1.f};
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 world_up;
    glm::vec3 velocity{0.f, 0.f, 0.f};
    GLfloat x_rotation{0.f};
    GLfloat y_rotation{0.f};

    GLfloat pitch;
    GLfloat yaw;
    GLfloat movement_speed;
    GLfloat rotation_speed;
};