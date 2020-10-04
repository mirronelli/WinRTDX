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

	std::unique_ptr<Scene> SceneFactory::LoadFromFile(std::string fileName, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, int& lastResourceID)
	{
		Assimp::Importer importer;
		const aiScene* sourceScene = importer.ReadFile(fileName,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);


		if (sourceScene != nullptr && sourceScene->HasMeshes())
		{
			std::unique_ptr<Scene> newScene = std::make_unique<Scene>(fileName);
			SceneFactory::LoadMeshesToScene(newScene.get(), sourceScene->mRootNode, sourceScene, vertexShader, pixelShader, lastResourceID);
			return newScene;
		}
		else
		{
			throw "Error loading mesh.";
		}
	}

	void SceneFactory::LoadMeshesToScene(Scene* parentScene, aiNode* node, const aiScene* sourceScene, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, int& lastResourceID)
	{
		if (node->mNumMeshes > 0)
		{
			CopyMeshesToScene(parentScene, node, sourceScene, vertexShader, pixelShader, lastResourceID);
		}

		for (unsigned int childNodeIndex = 0; childNodeIndex < node->mNumChildren; childNodeIndex++)
		{
			aiNode* childNode = node->mChildren[childNodeIndex];
			std::unique_ptr<Scene> childScene = std::make_unique<Scene>(node->mName.C_Str());
			
			childScene->Transform(ConvertMatrix(childNode->mTransformation));
			LoadMeshesToScene(childScene.get(), childNode, sourceScene, vertexShader, pixelShader, lastResourceID);
			
			parentScene->AddScene(std::move(childScene));
		}
	}

	void SceneFactory::CopyMeshesToScene(Scene* parentScene, aiNode* node, const aiScene* sourceScene, std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<PixelShader> pixelShader, int& lastResourceID)
	{
		for (unsigned int meshInNodeIndex = 0; meshInNodeIndex < node->mNumMeshes; meshInNodeIndex++)
		{
			int meshInSceneIndex = node->mMeshes[meshInNodeIndex];
			aiMesh* sourceMesh = sourceScene->mMeshes[meshInSceneIndex];

			std::unique_ptr<MeshColored> newMesh = std::make_unique<MeshColored>(vertexShader, pixelShader, lastResourceID++);
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

	inline XMMATRIX SceneFactory::ConvertMatrix(aiMatrix4x4& source)
	{
		return XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&source));
	}
}