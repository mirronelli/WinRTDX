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
		aiMaterial* material = sourceScene->mMaterials[sourceMesh->mMaterialIndex];
		VertexType vertexType;

		std::shared_ptr<InputLayout>	inputLayout;
		std::shared_ptr<VertexShader> vertexShader;
		std::shared_ptr<PixelShader>	pixelShader;

		std::shared_ptr<IndexBuffer>			indexBuffer;
		std::shared_ptr<VertexBufferBase>	vertexBuffer;
		std::shared_ptr<Texture>				diffuseTexture;
		std::shared_ptr<Texture>				specularTexture;
		std::shared_ptr<Texture>				normalTexture;
		std::shared_ptr<Sampler>				sampler;

		// read cached buffers
		indexBuffer = IndexBuffer::Get(name);
		vertexBuffer = VertexBufferBase::Get(name);

		// check texture presence
		aiString aiTextureFileName;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &aiTextureFileName) == aiReturn_SUCCESS)
		{
			vertexType = VertexType::TexturedWithNormal;

			std::string textureFileName(aiTextureFileName.C_Str());
			diffuseTexture = Texture::Preload(textureFileName, to_hstring(std::string("Assets\\nano_textured\\") + textureFileName));
			sampler = Sampler::Create("wrapedSampler", D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP, 0);
		}
		else
		{
			vertexType = VertexType::SimpleWithNormal;
		}

		aiColor3D aiDiffuseColor;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, aiDiffuseColor);

		aiColor3D aiSpecularColor;
		material->Get(AI_MATKEY_COLOR_SPECULAR, aiSpecularColor);

		if (vertexBuffer == nullptr)
		{
			if (vertexType == VertexType::SimpleWithNormal)
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
			else if (vertexType == VertexType::TexturedWithNormal)
			{
				std::unique_ptr<std::vector<VertexTexturedWithNormal>> vertices = std::make_unique<std::vector<VertexTexturedWithNormal>>();
				vertices->reserve(sourceMesh->mNumVertices);

				for (unsigned int i = 0; i < sourceMesh->mNumVertices; i++)
				{
					aiVector3D& vertex = sourceMesh->mVertices[i];
					aiVector3D& normal = sourceMesh->mNormals[i];
					auto textureCoordinates = sourceMesh->mTextureCoords[0][i];

					vertices->push_back(
						{ DirectX::XMFLOAT3{ vertex.x, vertex.y, vertex.z }, XMFLOAT3{ normal.x, normal.y, normal.z }, XMFLOAT2{ textureCoordinates.x, textureCoordinates.y} }
					);
				}

				vertexBuffer = VertexBuffer<VertexTexturedWithNormal>::Create(name, std::move(vertices));
			}
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

		inputLayout = InputLayout::Get(vertexType);
		vertexShader = VertexShader::Get(vertexType);
		pixelShader = PixelShader::Get(vertexType);

		std::unique_ptr<Mesh> newMesh = std::make_unique<Mesh>(
			name,
			inputLayout,
			vertexBuffer, 
			indexBuffer, 
			vertexShader, 
			pixelShader, 
			diffuseTexture,
			specularTexture,
			normalTexture,
			sampler
		);

		newMesh->Color({ aiDiffuseColor.r, aiDiffuseColor.g, aiDiffuseColor.b, 0 });
		newMesh->Specular({ aiSpecularColor.r, aiSpecularColor.g, aiSpecularColor.b, 1 }, 32);
		newMesh->Init();

		return newMesh;
	}

	inline XMMATRIX SceneFactory::ConvertMatrix(aiMatrix4x4& source)
	{
		return XMLoadFloat4x4(reinterpret_cast<XMFLOAT4X4*>(&source));
	}
}
