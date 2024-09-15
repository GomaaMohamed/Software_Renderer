#include "Mesh.hpp"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>
#include <assimp/matrix4x4.h>
#include <iostream>

namespace
{
    const unsigned int ImportFlags =
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_SortByPType |
        aiProcess_PreTransformVertices |
        aiProcess_GenNormals |
        aiProcess_GenUVCoords |
        aiProcess_OptimizeMeshes |
        aiProcess_Debone |
        aiProcess_ValidateDataStructure;
}

struct LogStream : public Assimp::LogStream
{
    static void initialize()
    {
        if (Assimp::DefaultLogger::isNullLogger())
        {
            Assimp::DefaultLogger::create("ARE Mesh Loader", Assimp::Logger::NORMAL);
            Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
        }
    }
    void write(const char *message) override
    {
        std::cout << "Assimp error:" << message << "\n";
    }
};

namespace Soft
{

    Mesh::Mesh(const std::string path)
    {
        LogStream::initialize();

        std::cout << "Loading mesh: " << path << "\n";

        Assimp::Importer importer;

        const aiScene *scene = importer.ReadFile(path, ImportFlags);
        if (!scene || !scene->HasMeshes())
            std::cout << "Failed to load mesh file: " << path << "\n";

        aiMesh *mesh = scene->mMeshes[0];

        // ASSERT(mesh->HasPositions(), "Meshes require positions.");
        // ASSERT(mesh->HasNormals(), "Meshes require normals.");

        vertices_.reserve(mesh->mNumVertices);

        // Extract vertices from model
        for (size_t i = 0; i < vertices_.capacity(); i++)
        {
            TexturedVertexEffect::Vertex vertex;
            vertex.x = mesh->mVertices[i].x;
            vertex.y = mesh->mVertices[i].y;
            vertex.z = mesh->mVertices[i].z;

            /*
            vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

            if (mesh->HasTangentsAndBitangents())
            {
                vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
                vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
            }
            */

            if (mesh->HasTextureCoords(0))
            {
                vertex.u = mesh->mTextureCoords[0][i].x;
                vertex.v = mesh->mTextureCoords[0][i].y;
            }

            vertices_.push_back(vertex);
        }

        // Extract indices from model
        indices_.reserve(mesh->mNumFaces);
        for (size_t i = 0; i < mesh->mNumFaces; i++)
        {
            indices_.push_back(mesh->mFaces[i].mIndices[0]);
            indices_.push_back(mesh->mFaces[i].mIndices[1]);
            indices_.push_back(mesh->mFaces[i].mIndices[2]);
        }
    }
    uint32_t Mesh::get_indices_count()
    {
        return indices_.size() * INDICES_PER_TRIANGLE;
    }
    const std::vector<TexturedVertexEffect::Vertex> &Mesh::get_vertices()
    {
        return vertices_;
    }
    const std::vector<uint32_t> &Mesh::get_indices()
    {
        return indices_;
    }
}