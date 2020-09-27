#pragma once
#include <pch.h>
#include <assimp\Importer.hpp>
#include <assimp/scene.h>           
#include <assimp/postprocess.h>

#include "Drawable.h"
#include "Structures.h"

using namespace Dx::Drawables;
namespace Dx
{
	class Mesh : public Drawable
	{
	public:
		using Drawable::Drawable;

		void Prepare()
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile("Assets\\suzanne.obj",
				aiProcess_CalcTangentSpace |
				aiProcess_Triangulate |
				aiProcess_JoinIdenticalVertices |
				aiProcess_SortByPType);

			if (scene->HasMeshes())
			{
				aiMesh* mesh = scene->mMeshes[0];

				for (int i = 0; i < mesh->mNumVertices; i++)
				{
					aiVector3D& vertex = mesh->mVertices[i];
					aiVector3D& normal = mesh->mNormals[i];
					m_vertices.push_back(
						{ XMFLOAT3{ vertex.x, vertex.y, vertex.z }, XMFLOAT3{ normal.x, normal.y, normal.z }, XMFLOAT3{ .5, .50, .5f} }
					);
				}

				for (int i = 0; i < mesh->mNumFaces; i++)
				{
					aiFace& face = mesh->mFaces[i];
					m_indices.push_back(face.mIndices[0]);
					m_indices.push_back(face.mIndices[1]);
					m_indices.push_back(face.mIndices[2]);
				}
			}

			m_drawableBuffer.reflectiveness = 3.5f;
			m_drawableBuffer.reflectionPower = 32;

			m_prepared = true;
		}

	protected:
		void RegisterResources() 
		{
			m_vertexBuffer = VertexBuffer<VertexWithNormalColor>						::Create(m_resourceCacheID, false, m_graphics, m_vertices);
			m_indexBuffer = IndexBuffer														::Create(m_resourceCacheID, false, m_graphics, m_indices);
			m_vsConstantBuffer = VSConstantBuffer<WorldTransformWithSpecular>		::Create(m_resourceCacheID, false, m_graphics, m_drawableBuffer, 2);
			m_psConstantBuffer = PSConstantBuffer<WorldTransformWithSpecular>		::Create(m_resourceCacheID, false, m_graphics, m_drawableBuffer, 2);
			m_inputLayout = InputLayout														::Create(m_resourceCacheID, false, m_graphics, IedsWithNormalColor, m_vertexShader);
			m_indicesCount = (UINT)m_indices.size();
		}
		void UpdateConstants(DirectX::CXMMATRIX worldTransform)
		{
			m_drawableBuffer.worldTransform = worldTransform;
			std::static_pointer_cast<VSConstantBuffer<WorldTransformWithSpecular >> (m_vsConstantBuffer)->Update(m_drawableBuffer);
			std::static_pointer_cast<PSConstantBuffer<WorldTransformWithSpecular >> (m_psConstantBuffer)->Update(m_drawableBuffer);
		}

	private:
		std::vector<VertexWithNormalColor>			m_vertices;
		std::vector<UINT>									m_indices;
		WorldTransformWithSpecular						m_drawableBuffer;
	};
}
