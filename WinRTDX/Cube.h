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

		inline static std::vector<Dx::Drawables::VertexSimple> Vertices {
			{ DirectX::XMFLOAT3(-1.0f,	 1.0f,	-1.0f) },
			{ DirectX::XMFLOAT3(1.0f,	 1.0f,	-1.0f) },
			{ DirectX::XMFLOAT3(1.0f,	-1.0f,	-1.0f) },
			{ DirectX::XMFLOAT3(-1.0f,	-1.0f,	-1.0f) },
			{ DirectX::XMFLOAT3(-1.0f,	 1.0f,	 1.0f) },
			{ DirectX::XMFLOAT3(1.0f,	 1.0f,	 1.0f) },
			{ DirectX::XMFLOAT3(1.0f,	-1.0f,	 1.0f) },
			{ DirectX::XMFLOAT3(-1.0f,	-1.0f,	 1.0f) }
		};

		inline static 	std::vector<UINT> Indices = {
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
		};

		void RegisterResources() {
			mPsConstants = {
					DirectX::XMFLOAT4(1.f, 0.5f, 0.5f, 1.f),
					DirectX::XMFLOAT4(0.5f, 1.f, 0.5f, 1.f),
					DirectX::XMFLOAT4(0.5f, 0.5f, 1.f, 1.f),
					DirectX::XMFLOAT4(1.f, 0.5f, 1.f, 1.f),
					DirectX::XMFLOAT4(0.5f, 1.f, 1.f, 1.f),
					DirectX::XMFLOAT4(1.f, 1.f, 0.5f, 1.f),
			};

			//mVertexBuffer =		VertexBuffer<VertexSimple>::			Get("cube", Vertices);
			//mIndexBuffer =		IndexBuffer::								Get("cube", Indices);
			mPsConstantBuffer =	PSConstantBuffer<PSConstants>::		Create(mPsConstants, ResourceSlots::PerInstance);
			mVsConstantBuffer =	VSConstantBuffer<WorldTransform>::	Create(mVsConstants, ResourceSlots::PerInstance);
			mInputLayout =		InputLayout::								Get(VertexType::Simple);
			mIndicesCount =		(UINT)Indices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			mVsConstants.worldTransform = matrix;			
			std::static_pointer_cast<VSConstantBuffer<WorldTransform >> (mVsConstantBuffer)->Update(mVsConstants);
		}

	private:
		PSConstants															mPsConstants = {};
		WorldTransform														mVsConstants = {};
	};
}
