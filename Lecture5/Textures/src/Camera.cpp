#include <Camera.hpp>

Camera::Camera(const glm::vec3& _position, const glm::vec3& _world_up, GLfloat _pitch, GLfloat _yaw, GLfloat _movement_speed, GLfloat _rotation_speed) noexcept
    : position{_position}, world_up{_world_up}, pitch{_pitch}, yaw{_yaw}, movement_speed{_movement_speed}, rotation_speed{_rotation_speed}
{
    update_vectors();
}

Camera::~Camera()
{

}

glm::mat4 Camera::get_view_matrix() const noexcept
{
    return glm::lookAt(position, position + front, up);
}

void Camera::handle_keys(const std::array<bool, 1024>& keys) noexcept
{
    if (keys[GLFW_KEY_W])
    {
        velocity = front * movement_speed;
    }

    if (keys[GLFW_KEY_S])
    {
        velocity = -front * movement_speed;
    }

    if (keys[GLFW_KEY_D])
    {
        velocity = right * movement_speed;
    }

    if (keys[GLFW_KEY_A])
    {
        velocity = -right * movement_speed;
    }
}

void Camera::handle_mouse(GLfloat x_change, GLfloat y_change) noexcept
{
    x_rotation = x_change * rotation_speed;
    y_rotation = -y_change * rotation_speed;
}

void Camera::update(GLfloat dt) noexcept
{
    position += velocity * dt;
    pitch += y_rotation * dt;
    yaw += x_rotation * dt;

    pitch = std::max(-89.f, std::min(89.f, pitch));

    update_vectors();
    velocity = glm::vec3{0.f, 0.f, 0.f};
    x_rotation = 0.f;
    y_rotation = 0.f;
}

void Camera::update_vectors() noexcept
{
    front.x = glm::cos(glm::radians(pitch)) * glm::cos(glm::radians(yaw));
    front.y = glm::sin(glm::radians(pitch));
    front.z = glm::cos(glm::radians(pitch)) * glm::sin(glm::radians(yaw));
    front = glm::normalize(front);

    right = glm::normalize(glm::cross(front, world_up));
    up = glm::normalize(glm::cross(right, front));
}
