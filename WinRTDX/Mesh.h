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

		void Prepare()
		{
			if ((m_meshData = m_loadedMeshes[m_meshFileName]) == nullptr)
			{
				LoadFromFile();
				m_loadedMeshes[m_meshFileName] = m_meshData;
			}

			m_perInstanceConstants.reflectiveness = 3.5f;
			m_perInstanceConstants.reflectionPower = 32;

			m_prepared = true;
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
						{ XMFLOAT3{ vertex.x, vertex.y, vertex.z }, XMFLOAT3{ normal.x, normal.y, normal.z }, XMFLOAT3{ .5, .50, .5f} }
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
			m_vertexBuffer = VertexBuffer<VertexWithNormalColor>						::Create(m_resourceCacheID, false, m_graphics, m_meshData->Vertices);
			m_indexBuffer = IndexBuffer														::Create(m_resourceCacheID, false, m_graphics, m_meshData->Indices);
			m_vsConstantBuffer = VSConstantBuffer<WorldTransformWithSpecular>		::Create(m_resourceCacheID, false, m_graphics, m_perInstanceConstants, (UINT)ResourceSlots::PerInstance);
			m_psConstantBuffer = PSConstantBuffer<WorldTransformWithSpecular>		::Create(m_resourceCacheID, false, m_graphics, m_perInstanceConstants, (UINT)ResourceSlots::PerInstance);
			m_inputLayout = InputLayout														::Create(m_resourceCacheID, false, m_graphics, IedsWithNormalColor, m_vertexShader);
			m_indicesCount = (UINT)m_meshData->Indices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX worldTransform)
		{
			m_perInstanceConstants.worldTransform = worldTransform;
			std::static_pointer_cast<VSConstantBuffer<WorldTransformWithSpecular >> (m_vsConstantBuffer)->Update(m_perInstanceConstants);
			std::static_pointer_cast<PSConstantBuffer<WorldTransformWithSpecular >> (m_psConstantBuffer)->Update(m_perInstanceConstants);
		}

	private:
		
		struct MeshData {
			std::vector<VertexWithNormalColor>		Vertices;
			std::vector<UINT>								Indices;
		};

		static inline std::map<std::string, std::shared_ptr<MeshData>>	m_loadedMeshes;
		std::shared_ptr<MeshData>											m_meshData;

		WorldTransformWithSpecular											m_perInstanceConstants;
		std::string																m_meshFileName;
	};
}
