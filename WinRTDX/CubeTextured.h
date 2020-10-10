#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>

using namespace Dx::Attachables;
using namespace DirectX;

namespace Dx::Drawables
{

	class CubeTextured : public Drawable
	{
	public:
		using Drawable::Drawable;

		void RegisterResources() {
			mPsConstants.reflectiveness = 1;
			mPsConstants.reflectionPower = 16;			

			std::string uuid = "cube:texturedWithNormal";

			if (nullptr == (mIndexBuffer = IndexBuffer::Get(uuid)))
			{
				std::unique_ptr<std::vector<UINT>> indices = std::make_unique<std::vector<unsigned int>>();
				std::unique_ptr<std::vector<VertexTexturedWithNormal>> vertices = std::make_unique<std::vector<VertexTexturedWithNormal>>();

				GenerateVerticesAndIndices(vertices, indices);
				mIndexBuffer = IndexBuffer::Create(uuid, std::move(indices));
				mVertexBuffer = VertexBuffer<VertexTexturedWithNormal>::Create(uuid, std::move(vertices));
			}
			else
			{
				mVertexBuffer = VertexBuffer<VertexTexturedWithNormal>::Get(uuid);
			}

			mInputLayout =		InputLayout::											Get(VertexType::TexturedWithNormal);
			mVsConstantBuffer =	VSConstantBuffer<WorldTransform>::				Create(mVsConstants, ResourceSlots::PerInstance);
			mPsConstantBuffer =	PSConstantBuffer<Specular>::						Create(mPsConstants, ResourceSlots::PerInstance);
		}

		void UpdateConstants(DirectX::CXMMATRIX viewPerspectiveTransform)
		{
			mVsConstants.worldTransform = viewPerspectiveTransform;
			std::static_pointer_cast<VSConstantBuffer<WorldTransform >> (mVsConstantBuffer)->Update(mVsConstants);
		}

		void GenerateVerticesAndIndices(std::unique_ptr<std::vector<VertexTexturedWithNormal>>& vertices, std::unique_ptr < std::vector< unsigned int> >& indices)
		{
			vertices->insert(
				vertices->begin(),
				{
					{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)},  // 0
					{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.5f,  .0f)},  // 1
					{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.5f,  .5f)},  // 2
					{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT2(0.0f,  .5f)},  // 3

					{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.5f, 0.0f)},  // 4
					{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f,  .0f)},  // 5
					{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f,  .5f)},  // 6
					{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT2(0.5f,  .5f)},  // 7

					{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f, 0.0f)},  // 8
					{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.5f,  .0f)},  // 9
					{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(0.5f,  .5f)},  // 10
					{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(-1.0f,  0.0f,  0.0f), XMFLOAT2(1.0f,  .5f)},  // 11

					{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f, 0.0f)},  // 12
					{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.5f,  .0f)},  // 13
					{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.5f,  .5f)},  // 14
					{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT2(0.0f,  .5f)},  // 15

					{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.5f, 0.5f)},  // 16
					{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 0.5f)},  // 17
					{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(0.5f, 1.0f)},  // 18
					{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT2(1.0f, 1.0f)},  // 19

					{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 0.5f)},  // 20
					{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.5f, 0.5f)},  // 21
					{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.0f, 1.0f)},  // 22
					{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT2(0.5f, 1.0f)},  // 23
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
		WorldTransform						mVsConstants = {};
		Specular								mPsConstants = {};
	};
}
