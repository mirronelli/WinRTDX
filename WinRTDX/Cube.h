#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace Dx::Attachables;
namespace Dx::Drawables
{
	class Cube : public Drawable
	{
		using Drawable::Drawable;

	public:
		typedef struct {
			DirectX::XMFLOAT4 colors[6]; 
		} PSConstants;

		void RegisterResources() {
			mPsConstants = {
					DirectX::XMFLOAT4(1.f, 0.5f, 0.5f, 1.f),
					DirectX::XMFLOAT4(0.5f, 1.f, 0.5f, 1.f),
					DirectX::XMFLOAT4(0.5f, 0.5f, 1.f, 1.f),
					DirectX::XMFLOAT4(1.f, 0.5f, 1.f, 1.f),
					DirectX::XMFLOAT4(0.5f, 1.f, 1.f, 1.f),
					DirectX::XMFLOAT4(1.f, 1.f, 0.5f, 1.f),
			};

			std::string uuid = "cube:simple";

			if (nullptr == (mIndexBuffer = IndexBuffer::Get(uuid)))
			{
				std::unique_ptr<std::vector<UINT>> indices = std::make_unique<std::vector<unsigned int>>();
				std::unique_ptr<std::vector<VertexSimple>> vertices = std::make_unique<std::vector<VertexSimple>>();

				GenerateVerticesAndIndices(vertices, indices);
				mIndexBuffer = IndexBuffer::Create(uuid, std::move(indices));
				mVertexBuffer = VertexBuffer<VertexSimple>::Create(uuid, std::move(vertices));
			}
			else
			{
				mVertexBuffer = VertexBuffer<VertexSimple>::Get(uuid);
			}

			mPsConstantBuffer =	PSConstantBuffer<PSConstants>::		Create(mPsConstants, ResourceSlots::PerInstance);
			mVsConstantBuffer =	VSConstantBuffer<WorldTransform>::	Create(mVsConstants, ResourceSlots::PerInstance);
			mInputLayout =		InputLayout::								Get(VertexType::Simple);
			mIndicesCount =	mIndexBuffer->Count();
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			mVsConstants.worldTransform = matrix;			
			std::static_pointer_cast<VSConstantBuffer<WorldTransform >> (mVsConstantBuffer)->Update(mVsConstants);
		}

	private:
		void GenerateVerticesAndIndices(std::unique_ptr<std::vector<VertexSimple>>& vertices, std::unique_ptr <std::vector<unsigned int>>& indices)
		{
			vertices->insert(
				vertices->begin(),
				{
					{ DirectX::XMFLOAT3(-1.0f,	 1.0f,	-1.0f) },
					{ DirectX::XMFLOAT3(1.0f,	 1.0f,	-1.0f) },
					{ DirectX::XMFLOAT3(1.0f,	-1.0f,	-1.0f) },
					{ DirectX::XMFLOAT3(-1.0f,	-1.0f,	-1.0f) },
					{ DirectX::XMFLOAT3(-1.0f,	 1.0f,	 1.0f) },
					{ DirectX::XMFLOAT3(1.0f,	 1.0f,	 1.0f) },
					{ DirectX::XMFLOAT3(1.0f,	-1.0f,	 1.0f) },
					{ DirectX::XMFLOAT3(-1.0f,	-1.0f,	 1.0f) }
				}
			);

			indices->insert(
				indices->begin(),
				{
					0, 1, 2,  // front
					0, 2, 3,
					1, 5, 2,  // right
					5, 6, 2,
					0, 7, 4,  // left
					0, 3, 7,
					0,	5, 1,  // top
					0, 4, 5,
					3, 2, 7,  // bottom
					2, 6, 7,
					4, 6, 5,  // back
					4, 7, 6
				}
			);
		}

		PSConstants															mPsConstants = {};
		WorldTransform														mVsConstants = {};
	};
}
