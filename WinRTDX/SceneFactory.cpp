#include "pch.h"
#include <assimp\Importer.hpp>
#include <assimp/scene.h>           
#include <assimp/postprocess.h>

#include "SceneFactory.h"
//#include "Mesh.h"

//using Dx::Drawables::Mesh;
using Dx::Drawables::Scene;
using Dx::Levels::SceneFactory;
using namespace DirectX;

std::unique_ptr<Scene> SceneFactory::LoadFromFile(std::string fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

		//meshData = std::make_shared<Mesh::MeshData>();

	if (scene != nullptr && scene->HasMeshes())
	{
		aiMesh* mesh = scene->mMeshes[0];

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D& vertex = mesh->mVertices[i];
			aiVector3D& normal = mesh->mNormals[i];

			//m_meshData->Vertices.push_back(
			//	{ XMFLOAT3{ vertex.x, vertex.y, vertex.z }, XMFLOAT3{ normal.x, normal.y, normal.z } }
			//);
		}

		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace& face = mesh->mFaces[i];
			//m_meshData->Indices.push_back(face.mIndices[0]);
			//m_meshData->Indices.push_back(face.mIndices[1]);
			//m_meshData->Indices.push_back(face.mIndices[2]);
		}
	}
	else
	{
		throw "Error loading mesh.";
	}
}