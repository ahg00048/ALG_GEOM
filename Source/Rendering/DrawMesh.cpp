#include "stdafx.h"
#include <assimp/postprocess.h>
#include "DrawMesh.h"
#include "Vect2d.h"

// Public methods

AlgGeom::DrawMesh::DrawMesh(TriangleModel& triangleModel) : Model3D()
{
    std::vector<Vect3d>* vertices = triangleModel.getVertices(), * normals = triangleModel.getNormals();
    std::vector<Vect2d>* textCoordinates = triangleModel.getTextureCoordinates();
    std::vector<unsigned>* indices = triangleModel.getIndices();
    const size_t numFaces = triangleModel.numTriangles();

    Component* component = new Component;

    for (size_t vertexIdx = 0; vertexIdx < vertices->size(); ++vertexIdx)
    {
        VAO::Vertex vertex{ vec3(vertices->at(vertexIdx).getX(), vertices->at(vertexIdx).getY(), vertices->at(vertexIdx).getZ()),
                            vec3(normals->at(vertexIdx).getX(), normals->at(vertexIdx).getY(), normals->at(vertexIdx).getZ()) };
        if (!textCoordinates->empty())
            vertex._textCoord = vec2(textCoordinates->at(vertexIdx).getX(), textCoordinates->at(vertexIdx).getY());
        component->_vertices.push_back(vertex);
    }

    for (size_t faceIdx = 0; faceIdx < numFaces; ++faceIdx)
    {
        component->_indices[VAO::IBO_TRIANGLE].insert(component->_indices[VAO::IBO_TRIANGLE].end(),
            { indices->at(faceIdx * 3 + 0), indices->at(faceIdx * 3 + 1), indices->at(faceIdx * 3 + 2), RESTART_PRIMITIVE_INDEX });
    }

    component->completeTopology();
    this->buildVao(component);
    this->_components.push_back(std::unique_ptr<Component>(component));
}

AlgGeom::DrawMesh::DrawMesh(std::vector<Triangle3d*>& triangles)
{
    Component* component = new Component;

    for (unsigned i = 0; i < triangles.size(); i++)
    {
        Triangle3d* tri = triangles[i];
        Vect3d triN = tri->normal();
        vec3 n(triN.getX(), triN.getY(), triN.getZ());

        component->_vertices.insert(component->_vertices.end(), {
                VAO::Vertex { vec3(tri->getA().getX(), tri->getA().getY(), tri->getA().getZ()), n },
                VAO::Vertex { vec3(tri->getB().getX(), tri->getB().getY(), tri->getB().getZ()), n },
                VAO::Vertex { vec3(tri->getC().getX(), tri->getC().getY(), tri->getC().getZ()), n }
            });

        unsigned offset = 3;
        component->_indices[VAO::IBO_TRIANGLE].insert(component->_indices[VAO::IBO_TRIANGLE].end(), { 0 + offset * i, 1 + offset * i, 2 + offset * i, RESTART_PRIMITIVE_INDEX });
    }

    component->completeTopology();

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}


AlgGeom::DrawMesh::DrawMesh(std::vector<Triangle3d>& triangles)
{
    Component* component = new Component;

    for (unsigned i = 0; i < triangles.size(); i++)
    {
        Triangle3d* tri = &triangles[i];
        Vect3d triN = tri->normal();
        vec3 n(triN.getX(), triN.getY(), triN.getZ());

        component->_vertices.insert(component->_vertices.end(), {
                VAO::Vertex { vec3(tri->getA().getX(), tri->getA().getY(), tri->getA().getZ()), n },
                VAO::Vertex { vec3(tri->getB().getX(), tri->getB().getY(), tri->getB().getZ()), n },
                VAO::Vertex { vec3(tri->getC().getX(), tri->getC().getY(), tri->getC().getZ()), n }
            });

        unsigned offset = 3;
        component->_indices[VAO::IBO_TRIANGLE].insert(component->_indices[VAO::IBO_TRIANGLE].end(), { 0 + offset * i, 1 + offset * i, 2 + offset * i, RESTART_PRIMITIVE_INDEX });
    }

    component->completeTopology();

    this->_components.push_back(std::unique_ptr<Component>(component));
    this->buildVao(component);
}

// Protected methods

AlgGeom::Model3D::Component* AlgGeom::DrawMesh::processMesh(aiMesh* mesh, const aiScene* scene, const std::string& folder)
{
    std::vector<VAO::Vertex> vertices(mesh->mNumVertices);
    std::vector<GLuint> indices(mesh->mNumFaces * 4);

    // Vertices
    int numVertices = static_cast<int>(mesh->mNumVertices);

    for (int i = 0; i < numVertices; i++)
    {
        VAO::Vertex vertex;
        vertex._position = vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex._normal = vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        if (mesh->mTextureCoords[0]) vertex._textCoord = vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        vertices[i] = vertex;
    }

    // Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices[i * 4 + j] = face.mIndices[j];

        indices[i * 4 + 3] = RESTART_PRIMITIVE_INDEX;
    }

    Component* component = new Component;
    component->_vertices = std::move(vertices);
    component->_indices[VAO::IBO_TRIANGLE] = std::move(indices);
    component->completeTopology();

    return component;
}

void AlgGeom::DrawMesh::processNode(aiNode* node, const aiScene* scene, const std::string& folder)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        _components.push_back(std::unique_ptr<Component>(this->processMesh(mesh, scene, folder)));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene, folder);
    }
}

AlgGeom::DrawMesh* AlgGeom::DrawMesh::loadModelOBJ(const std::string& path)
{
    std::string binaryFile = path.substr(0, path.find_last_of('.')) + BINARY_EXTENSION;

    if (std::filesystem::exists(binaryFile))
    {
        loadModelBinaryFile(binaryFile);
    }
    else
    {
        const aiScene* scene = _assimpImporter.ReadFile(path, aiProcess_JoinIdenticalVertices | aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << _assimpImporter.GetErrorString() << std::endl;
            return this;
        }

        std::string shortName = scene->GetShortFilename(path.c_str());
        std::string folder = path.substr(0, path.length() - shortName.length());

        processNode(scene->mRootNode, scene, folder);
        writeBinaryFile(binaryFile);
    }

    for (auto& component : _components)
    {
        this->buildVao(component.get());
    }

    return this;
}
