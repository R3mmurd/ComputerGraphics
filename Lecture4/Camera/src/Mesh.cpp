#include <Mesh.hpp>

std::shared_ptr<Mesh> Mesh::create(const std::vector<GLfloat>& vertices, std::vector<unsigned int>& indices) noexcept
{
    auto mesh = std::make_shared<Mesh>();

    mesh->index_count = indices.size();

    glGenVertexArrays(1, &mesh->VAO_id);
    glBindVertexArray(mesh->VAO_id);

    glGenBuffers(1, &mesh->IBO_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->IBO_id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mesh->VBO_id);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO_id);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    return mesh;
}

Mesh::~Mesh()
{
    clear();
}

void Mesh::render() const noexcept
{
    glBindVertexArray(VAO_id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_id);
    glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

void Mesh::clear() noexcept
{
    if (IBO_id != 0)
    {
        glDeleteBuffers(1, &IBO_id);
        IBO_id = 0;
    }

    if (VBO_id != 0)
    {
        glDeleteBuffers(1, &VBO_id);
        VBO_id = 0;
    }

    if (VAO_id != 0)
    {
        glDeleteBuffers(1, &VAO_id);
        VAO_id = 0;
    }
}
