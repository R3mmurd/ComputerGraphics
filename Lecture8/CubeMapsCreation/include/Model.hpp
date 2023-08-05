#pragma once

#include <filesystem>
#include <memory>
#include <string_view>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <BSlogger.hpp>

#include <Mesh.hpp>
#include <Texture.hpp>

class Model
{
public:
    Model(const std::filesystem::path& _root_path) noexcept;

    ~Model() {}

    void load(std::string_view model_name) noexcept;

    void render() const noexcept;

private:
    void load_node(aiNode* node, const aiScene* scene) noexcept;

    void load_mesh(aiMesh* mesh) noexcept;

    void load_textures(const aiScene* scene) noexcept;

    const std::filesystem::path& root_path;
    std::vector<std::shared_ptr<Mesh>> mesh_list{};
    std::vector<std::shared_ptr<Texture>> texture_list{};
    std::vector<unsigned int> mesh_to_texture{};
};
