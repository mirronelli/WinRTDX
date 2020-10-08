#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>

using namespace DirectX;
using namespace Dx::Attachables;
namespace Dx::Drawables
{

	class CubeColored : public Drawable
	{
		using Drawable::Drawable;

	public:
		struct VertexSimple {
			DirectX::XMFLOAT3	position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT3	color;
		};

		typedef struct {
			DirectX::XMMATRIX matrix;
		} VSConstants;

		void RegisterResources() {
			std::string uuid = "cube";

			if (nullptr == (mIndexBuffer = IndexBuffer::Get(uuid)))
			{
				std::unique_ptr<std::vector<UINT>> indices = std::make_unique<std::vector<unsigned int>>();
				std::unique_ptr<std::vector<VertexColoredWithNormal>> vertices = std::make_unique<std::vector<VertexColoredWithNormal>>();

				GenerateVerticesAndIndices(vertices, indices);
				mIndexBuffer = IndexBuffer::Create(uuid, std::move(indices));
				mVertexBuffer = VertexBuffer<VertexColoredWithNormal>::Create(uuid, std::move(vertices));
			}
			else
			{
				mVertexBuffer = VertexBuffer<VertexColoredWithNormal>::Get(uuid);
			}
			mVsConstantBuffer =	VSConstantBuffer<VSConstants>::	Create(mVsConstants, ResourceSlots::PerInstance);
			mInputLayout =		InputLayout::								Get(VertexType::ColoredWithNormal);
			mIndicesCount =		mIndexBuffer->Count();
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			mVsConstants.matrix = matrix;			
			std::static_pointer_cast<VSConstantBuffer<VSConstants >> (mVsConstantBuffer)->Update(mVsConstants);
		}

		void GenerateVerticesAndIndices(std::unique_ptr<std::vector<VertexColoredWithNormal>>& vertices, std::unique_ptr < std::vector< unsigned int> >& indices)
		{
			vertices->insert(
				vertices->begin(),	
				{
					{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT3(1.0f, 0.0f, 0.0f)},  // 0==============
					{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},  // 1
					{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT3(0.0f, 0.0f, 1.0f)},  // 2
					{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)},  // 3

					{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT3(1.0f, 1.0f, 0.0f)},  // 4==============
					{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT3(0.0f, 1.0f, 1.0f)},  // 5
					{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT3(1.0f, 0.0f, 1.0f)},  // 6
					{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)},  // 7

					{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f,  0.0f), XMFLOAT3(1.0f, 0.5f, 0.0f)},  // 8==============
					{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(-1.0f, 0.0f,  0.0f), XMFLOAT3(0.5f, 1.0f, 0.0f)},  // 9
					{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(-1.0f, 0.0f,  0.0f), XMFLOAT3(0.5f, 0.0f, 1.0f)},  // 10
					{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f,  0.0f), XMFLOAT3(1.0f, 0.0f, 0.5f)},  // 11

					{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT3(0.0f, 1.0f, 0.5f)},  // 12=============
					{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT3(0.0f, 0.5f, 1.0f)},  // 13
					{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT3(0.5f, 0.5f, 0.0f)},  // 14
					{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT3(0.0f, 0.5f, 0.5f)},  // 15

					{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3(0.5f, 0.0f, 0.5f)},  // 16=============
					{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3(1.0f, 0.5f, 1.0f)},  // 17
					{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3(1.0f, 1.0f, 0.5f)},  // 18
					{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3(0.5f, 1.0f, 1.0f)},  // 19

					{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT3(0.0f, 0.5f, 0.0f)},  // 20=============
					{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT3(0.5f, 0.5f, 0.0f)},  // 21
					{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT3(0.0f, 1.0f, 0.0f)},  // 22
					{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT3(0.5f, 1.0f, 0.0f)},  // 23
				}
			);

			indices->insert(
				indices->begin(),
				{
				0, 1, 2,  // front
				0, 2, 3,

				4, 5, 6,  // right
				4, 6, 7,

				10, 9, 8,  // left
				11, 10, 8,

				14, 13, 12, // back
				15, 14, 12,

				17, 16, 18, // top
				17, 18, 19,

				22, 20, 21, // bottom
				23, 22, 21
				}
			);
		}

	private:
		VSConstants															mVsConstants = {};
	};
}
