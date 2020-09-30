#pragma once
#include <pch.h>
#include <assimp\Importer.hpp>
#include <assimp/scene.h>           
#include <assimp/postprocess.h>
#include <map>

#include "Drawable.h"
#include "Structures.h"

using namespace Dx::Drawables;
using namespace Dx::Attachables;
namespace Dx
{
	class Mesh : public Drawable
	{
	public:
		using Drawable::Drawable;

		void FileName(std::string fileName) { m_meshFileName = fileName; }

		void Color(XMFLOAT4 color) { m_color = color; }

		void Specular(XMFLOAT2 specular) { m_specular = specular; }

		void Init()
		{
			if ((m_meshData = m_loadedMeshes[m_meshFileName]) == nullptr)
			{
				LoadFromFile();
				m_loadedMeshes[m_meshFileName] = m_meshData;
			}

			m_pixelPerInstanceConstants.color = m_color;
			m_pixelPerInstanceConstants.reflectiveness = m_specular.x;
			m_pixelPerInstanceConstants.reflectionPower = m_specular.y;

			mInitialized = true;
		}

	protected:
		void LoadFromFile()
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(m_meshFileName,
				aiProcess_CalcTangentSpace |
				aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType);

			m_meshData = std::make_shared<MeshData>();

			if (scene!= nullptr && scene->HasMeshes())
			{
				aiMesh* mesh = scene->mMeshes[0];

				for (unsigned int i = 0; i < mesh->mNumVertices; i++)
				{
					aiVector3D& vertex = mesh->mVertices[i];
					aiVector3D& normal = mesh->mNormals[i];

					m_meshData->Vertices.push_back(
						{ XMFLOAT3{ vertex.x, vertex.y, vertex.z }, XMFLOAT3{ normal.x, normal.y, normal.z } }
					);
				}

				for (unsigned int i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace& face = mesh->mFaces[i];
					m_meshData->Indices.push_back(face.mIndices[0]);
					m_meshData->Indices.push_back(face.mIndices[1]);
					m_meshData->Indices.push_back(face.mIndices[2]);
				}
			}
			else
			{
				throw "Error loading mesh.";
			}
		}

		void RegisterResources() 
		{
			m_vertexBuffer = VertexBuffer<VertexWithNormal>								::Create(m_resourceCacheID, false, m_graphics, m_meshData->Vertices);
			m_indexBuffer = IndexBuffer														::Create(m_resourceCacheID, false, m_graphics, m_meshData->Indices);
			m_vsConstantBuffer = VSConstantBuffer<WorldTransform>						::Create(m_resourceCacheID, false, m_graphics, m_vertexPerInstanceConstants, (UINT)ResourceSlots::PerInstance);
			m_psConstantBuffer = PSConstantBuffer<ColorSpecular>						::Create(m_resourceCacheID, false, m_graphics, m_pixelPerInstanceConstants, (UINT)ResourceSlots::PerInstance);
			m_inputLayout = InputLayout														::Create(m_resourceCacheID, false, m_graphics, IedsWithNormal, m_vertexShader);
			m_indicesCount = (UINT)m_meshData->Indices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX worldTransform)
		{
			m_vertexPerInstanceConstants.worldTransform = worldTransform;

			std::static_pointer_cast<VSConstantBuffer<WorldTransform>>		(m_vsConstantBuffer)->Update(m_vertexPerInstanceConstants);
			std::static_pointer_cast<PSConstantBuffer<ColorSpecular>>		(m_psConstantBuffer)->Update(m_pixelPerInstanceConstants);
		}

	private:
		
		struct MeshData {
			std::vector<VertexWithNormal>				Vertices;
			std::vector<UINT>								Indices;
		};

		static inline std::map<std::string, std::shared_ptr<MeshData>>	m_loadedMeshes;
		std::shared_ptr<MeshData>													m_meshData;

		ColorSpecular																	m_pixelPerInstanceConstants;
		WorldTransform																	m_vertexPerInstanceConstants;
		std::string																		m_meshFileName;
		XMFLOAT4																			m_color;
		XMFLOAT2																			m_specular;
	};
}
