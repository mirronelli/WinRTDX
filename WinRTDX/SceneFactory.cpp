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
			SceneFactory::LoadMeshesToScene(newScene.get(), sourceScene->mRootNode, DirectX::XMMatrixIdentity(), sourceScene, vertexShader, pixelShader);
			return newScene;
		}
		else
		{
			throw "Error loading mesh.";
		}
	}

	void SceneFactory::LoadMeshesToScene(Scene* parentScene, aiNode* node, DirectX::CXMMATRIX parentMatrix, const aiScene* sourceScene, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader)
	{
		if (node->mNumMeshes > 0)
		{
			CopyMeshesToScene(parentScene, node, sourceScene, vertexShader, pixelShader);
		}
	}

	void SceneFactory::CopyMeshesToScene(Scene* parentScene, aiNode* node, const aiScene* sourceScene, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader)
	{
		for (int meshInNodeIndex = 0; meshInNodeIndex < node->mNumMeshes; meshInNodeIndex++)
		{
			int meshInSceneIndex = node->mMeshes[meshInNodeIndex];
			aiMesh* sourceMesh = sourceScene->mMeshes[meshInSceneIndex];

			std::unique_ptr<MeshColored> newMesh = std::make_unique<MeshColored>(vertexShader, pixelShader, 10);
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

			parentScene->AddDrawable(std::move(newMesh));
		}
}