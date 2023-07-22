#include <Material.hpp>

Material::Material(GLfloat _specular_intensity, GLfloat _shininess) noexcept
    : specular_intensity{_specular_intensity}, shininess{_shininess}
{

}

Material::~Material()
{
    
}

void Material::use(GLuint specular_intensity_id, GLuint specular_shininess_id) const noexcept
{
    glUniform1f(specular_intensity_id, specular_intensity);
    glUniform1f(specular_shininess_id, shininess);
}