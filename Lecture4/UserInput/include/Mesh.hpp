#pragma once

#include <memory>
#include <vector>

#include <GL/glew.h>

class Mesh
{
public:
    Mesh() = default;

    static std::shared_ptr<Mesh> create(const std::vector<GLfloat>& vertices, std::vector<unsigned int>& indices) noexcept;

    Mesh(const Mesh& mesh) = delete;

    Mesh(Mesh&& mesh) = delete;

    ~Mesh();

    Mesh& operator = (const Mesh& mesh) = delete;

    Mesh& operator = (Mesh&& mesh) = delete;

    void render() const noexcept;
    
private:
    void clear() noexcept;

    GLuint VAO_id{0};
    GLuint VBO_id{0};
    GLuint IBO_id{0};
    GLsizei index_count{0};
};