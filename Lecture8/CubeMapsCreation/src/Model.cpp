#include <Model.hpp>

Model::Model(const std::filesystem::path& _root_path) noexcept
    : root_path{_root_path}
{

}

void Model::load(std::string_view model_name) noexcept
{
    Assimp::Importer importer{};

    const aiScene* scene = importer.ReadFile(root_path / "models" / model_name, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

    if (!scene)
    {
        LOG_INIT_CERR();
        log(LOG_ERR) << "Failed to load the model " << model_name << " " << importer.GetErrorString() << "\n";
        return;
    }

    load_node(scene->mRootNode, scene);

    load_textures(scene);
}

void Model::render() const noexcept
{
    for (size_t i = 0; i < mesh_list.size(); ++i)
    {
        unsigned int t_i = mesh_to_texture[i];
        
        if (t_i < texture_list.size() && texture_list[t_i])
        {
            texture_list[t_i]->use();
        }

        mesh_list[i]->render();
    }
}

void Model::load_node(aiNode* node, const aiScene* scene) noexcept
{
    for (size_t i = 0; i < node->mNumMeshes; ++i)
    {
        load_mesh(scene->mMeshes[node->mMeshes[i]]);
    }

    for (size_t i = 0; i < node->mNumChildren; ++i)
    {
        load_node(node->mChildren[i], scene);
    }
}

void Model::load_mesh(aiMesh* mesh) noexcept
{
    std::vector<GLfloat> vertices;
    std::vector<unsigned int> indices;

    for (size_t i = 0; i < mesh->mNumVertices; ++i)
    {
        vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });

        if (mesh->mTextureCoords[0])
        {
            vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
        }
        else
        {
            vertices.insert(vertices.end(), { 0.f, 0.f });
        }

        vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
    }

    for (size_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];

        for (size_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    mesh_list.push_back(Mesh::create(vertices, indices));
    mesh_to_texture.push_back(mesh->mMaterialIndex);
}

void Model::load_textures(const aiScene* scene) noexcept
{
    for (size_t i = 0; i < scene->mNumMaterials; ++i)
    {
        aiMaterial* material = scene->mMaterials[i];

        std::shared_ptr<Texture> texture{nullptr};

        if (material->GetTextureCount(aiTextureType_DIFFUSE))
        {
            aiString ai_path_str;

            if (material->GetTexture(aiTextureType_DIFFUSE, 0, &ai_path_str) == AI_SUCCESS)
            {
                std::string_view path_sv{ai_path_str.data};
                auto last_backslash_idx = path_sv.rfind("\\");
                std::filesystem::path texture_path{root_path / "textures" / path_sv.substr(last_backslash_idx + 1)};

                std::string ext = texture_path.extension();
                std::transform(ext.begin(), ext.end(), ext.begin(), [](auto c) { return std::tolower(c); });
                texture_path.replace_extension(ext);

                texture = std::make_shared<Texture>(texture_path);

                if (texture->load())
                {
                    texture_list.push_back(texture);
                    return;
                }
            }
        }

        texture = std::make_shared<Texture>(root_path / "textures" / "plain.png");
        texture->load_a();
        texture_list.push_back(texture);
    }
}