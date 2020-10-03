#include "pch.h"
#include <assimp\Importer.hpp>
#include <assimp/scene.h>           
#include <assimp/postprocess.h>

#include "SceneFactory.h"
#include "MeshColored.h"

//using Dx::Drawables::Mesh;
using namespace Dx::Drawables;
using namespace DirectX;

namespace Dx::Levels
{

	std::unique_ptr<Scene> SceneFactory::LoadFromFile(std::string fileName, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader)
	{
		Assimp::Importer importer;
		const aiScene* sourceScene = importer.ReadFile(fileName,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);


		if (sourceScene != nullptr && sourceScene->HasMeshes())
		{
			std::unique_ptr<Scene> newScene = std::make_unique<Scene>();
			std::unique_ptr<MeshColored> newMesh = std::make_unique<MeshColored>(vertexShader, pixelShader, 10);
			
			aiMesh* sourceMesh = sourceScene->mMeshes[0];
			newMesh->mVertices.reserve(sourceMesh->mNumVertices);

			for (unsigned int i = 0; i < sourceMesh->mNumVertices; i++)
			{
				aiVector3D& vertex = sourceMesh->mVertices[i];
				aiVector3D& normal = sourceMesh->mNormals[i];

				newMesh->mVertices.push_back(
					{ DirectX::XMFLOAT3{ vertex.x, vertex.y, vertex.z }, XMFLOAT3{ normal.x, normal.y, normal.z } }
				);
			}

			for (unsigned int i = 0; i < sourceMesh->mNumFaces; i++)
			{
				aiFace& sourceFace = sourceMesh->mFaces[i];
				newMesh->mIndices.push_back(sourceFace.mIndices[0]);
				newMesh->mIndices.push_back(sourceFace.mIndices[1]);
				newMesh->mIndices.push_back(sourceFace.mIndices[2]);
			}

			newMesh->Color({ 1,0,0,0 });
			newMesh->Specular(1, 32);
			newMesh->Init();
			newScene->AddDrawable(std::move(newMesh));
			return newScene;
		}
		else
		{
			throw "Error loading mesh.";
		}
	}
}