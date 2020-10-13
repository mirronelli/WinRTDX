#include "pch.h"
#include <assimp\Importer.hpp>
#include <assimp/scene.h>           
#include <assimp/postprocess.h>

#include "SceneFactory.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <fmt/core.h>

//using Dx::Drawables::Mesh;
using namespace Dx::Drawables;
using namespace DirectX;

namespace Dx::Levels
{

	std::unique_ptr<Scene> SceneFactory::Create(std::string fileName)
	{
		if (mScenes[fileName] != nullptr)
		{
			return mScenes[fileName]->Clone();
		}

		Assimp::Importer importer;
		const aiScene* sourceScene = importer.ReadFile(fileName,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);

		if (sourceScene != nullptr && sourceScene->HasMeshes())
		{
			std::shared_ptr<Scene> newScene = std::make_shared<Scene>(fileName);
			SceneFactory::ProcessNode(newScene.get(), sourceScene->mRootNode, sourceScene, fileName);
			mScenes[fileName] = newScene;
			
			return newScene->Clone();
		}
		else
		{
			throw "Error loading mesh.";
		}
	}

	void SceneFactory::ProcessNode(Scene* parentScene, aiNode* node, const aiScene* sourceScene, std::string baseName)
	{
		// for children meshes, copy them to scene
		if (node->mNumMeshes > 0)
		{
			CopyMeshesToScene(parentScene, node, sourceScene, baseName);
		}

		// for children nodes, create a scene for each node and process it recursively
		for (unsigned int childNodeIndex = 0; childNodeIndex < node->mNumChildren; childNodeIndex++)
		{
			aiNode* childNode = node->mChildren[childNodeIndex];
			std::string name = baseName + ":" + std::string(node->mName.C_Str());
			std::unique_ptr<Scene> childScene = std::make_unique<Scene>(name);
			
			childScene->Transform(ConvertMatrix(childNode->mTransformation));
			ProcessNode(childScene.get(), childNode, sourceScene, baseName);
			
			parentScene->AddScene(std::move(childScene));
		}
	}

	void SceneFactory::CopyMeshesToScene(Scene* parentScene, aiNode* node, const aiScene* sourceScene, std::string baseName)
	{
		for (unsigned int meshInNodeIndex = 0; meshInNodeIndex < node->mNumMeshes; meshInNodeIndex++)
		{
			int meshInSceneIndex = node->mMeshes[meshInNodeIndex];
			aiMesh* sourceMesh = sourceScene->mMeshes[meshInSceneIndex];
			std::unique_ptr<Mesh> newMesh = CreateMeshColored(sourceMesh, sourceScene, baseName);
			parentScene->AddDrawable(std::move(newMesh));
		}
	}

	std::unique_ptr <Mesh> SceneFactory::CreateMeshColored(aiMesh* sourceMesh, const aiScene* sourceScene, std::string baseName)
	{
		std::string name = baseName + ":" + std::string(sourceMesh->mName.C_Str());
		std::shared_ptr<InputLayout>		inputLayout = InputLayout::Get(VertexType::SimpleWithNormal);
		std::shared_ptr<VertexShader>		vertexShader = VertexShader::Get(VertexType::SimpleWithNormal);
		std::shared_ptr<PixelShader>		pixelShader = PixelShader::Get(VertexType::SimpleWithNormal);
		std::shared_ptr<IndexBuffer>		indexBuffer = IndexBuffer::Get(name);
		std::shared_ptr<VertexBuffer<VertexSimpleWithNormal>> vertexBuffer = VertexBuffer<VertexSimpleWithNormal>::Get(name);

		aiMaterial* material = sourceScene->mMaterials[sourceMesh->mMaterialIndex];

		std::string prop = fmt::format("------------------- Object: {}\n", name);
		OutputDebugStringA(prop.c_str());

		for (int i = 0; i < material->mNumProperties; i++)
		{
			auto property = material->mProperties[i];
			if (property->mDataLength > 0)
			{
				std::string prop = fmt::format("Property: {}, \tValue: {}\n", material->mProperties[i]->mKey.C_Str(), material->mProperties[i]->mDataLength);
				OutputDebugStringA(prop.c_str());
			}
		}

		aiString texFileName;
		std::shared_ptr<Texture> texture;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
		{
			std::string textureFileName(texFileName.C_Str());
			texture = Texture::Preload(textureFileName, to_hstring(std::string("Assets\\nano_textured\\") + textureFileName));
		};

		aiColor3D aiDiffuseColor;
		material->Get(AI_MATKEY_COLOR_TRANSPARENT, aiDiffuseColor);

		if (vertexBuffer == nullptr)
		{
			std::unique_ptr<std::vector<VertexSimpleWithNormal>> vertices = std::make_unique<std::vector<VertexSimpleWithNormal>>();
			vertices->reserve(sourceMesh->mNumVertices);

			for (unsigned int i = 0; i < sourceMesh->mNumVertices; i++)
			{
				aiVector3D& vertex = sourceMesh->mVertices[i];
				aiVector3D& normal = sourceMesh->mNormals[i];

				vertices->push_back(
					{ DirectX::XMFLOAT3{ vertex.x, vertex.y, vertex.z }, XMFLOAT3{ normal.x, normal.y, normal.z } }
				);
			}

			vertexBuffer = VertexBuffer<VertexSimpleWithNormal>::Create(name, std::move(vertices));
		}

		if (indexBuffer == nullptr)
		{
			std::unique_ptr<std::vector<unsigned int>> indices = std::make_unique<std::vector<unsigned int>>();
			indices->reserve(sourceMesh->mNumFaces * 3);

			for (unsigned int i = 0; i < sourceMesh->mNumFaces; i++)
			{
				aiFace& sourceFace = sourceMesh->mFaces[i];
				indices->push_back(sourceFace.mIndices[0]);
				indices->push_back(sourceFace.mIndices[1]);
				indices->push_back(sourceFace.mIndices[2]);
			}

			indexBuffer = IndexBuffer::Create(name, std::move(indices));
		}

		std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>(
			name,
			inputLayout,
			vertexBuffer, 
			indexBuffer, 
			vertexShader, 
			pixelShader
		);

		newMesh->Color({ aiDiffuseColor.r, aiDiffuseColor.g, aiDiffuseColor.b, 0 });
		newMesh->Specular(0, 1);
		newMesh->Init();

		return newMesh;
	}

	inline XMMATRIX SceneFactory::ConvertMatrix(aiMatrix4x4& source)
	{
		return XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&source));
	}
}