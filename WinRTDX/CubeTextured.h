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

		inline static std::vector<VertexTexturedWithNormal> Vertices  {
			{ XMFLOAT3( -1.0f,	 1.0f, -1.0f ), XMFLOAT3(  0.0f,  0.0f, -1.0f ), XMFLOAT2(  0.0f, 0.0f )},  // 0
			{ XMFLOAT3(  1.0f,	 1.0f, -1.0f ), XMFLOAT3(  0.0f,  0.0f, -1.0f ), XMFLOAT2(  0.5f,  .0f )},  // 1
			{ XMFLOAT3(  1.0f,	-1.0f, -1.0f ), XMFLOAT3(  0.0f,  0.0f, -1.0f ), XMFLOAT2(  0.5f,  .5f )},  // 2
			{ XMFLOAT3( -1.0f,	-1.0f, -1.0f ), XMFLOAT3(  0.0f,  0.0f, -1.0f ), XMFLOAT2(  0.0f,  .5f )},  // 3
																																		   
			{ XMFLOAT3(  1.0f,	 1.0f, -1.0f ), XMFLOAT3(  1.0f,  0.0f,  0.0f ), XMFLOAT2(  0.5f, 0.0f )},  // 4
			{ XMFLOAT3(  1.0f,	 1.0f,  1.0f ), XMFLOAT3(  1.0f,  0.0f,  0.0f ), XMFLOAT2(  1.0f,  .0f )},  // 5
			{ XMFLOAT3(  1.0f,	-1.0f,  1.0f ), XMFLOAT3(  1.0f,  0.0f,  0.0f ), XMFLOAT2(  1.0f,  .5f )},  // 6
			{ XMFLOAT3(  1.0f,	-1.0f, -1.0f ), XMFLOAT3(  1.0f,  0.0f,  0.0f ), XMFLOAT2(  0.5f,  .5f )},  // 7
																																		    
			{ XMFLOAT3( -1.0f,	 1.0f, -1.0f ), XMFLOAT3( -1.0f,  0.0f,  0.0f ), XMFLOAT2(  1.0f, 0.0f )},  // 8
			{ XMFLOAT3( -1.0f,	 1.0f,  1.0f ), XMFLOAT3( -1.0f,  0.0f,  0.0f ), XMFLOAT2(  0.5f,  .0f )},  // 9
			{ XMFLOAT3( -1.0f,	-1.0f,  1.0f ), XMFLOAT3( -1.0f,  0.0f,  0.0f ), XMFLOAT2(  0.5f,  .5f )},  // 10
			{ XMFLOAT3( -1.0f,	-1.0f, -1.0f ), XMFLOAT3( -1.0f,  0.0f,  0.0f ), XMFLOAT2(  1.0f,  .5f )},  // 11

			{ XMFLOAT3( -1.0f,	 1.0f,  1.0f ), XMFLOAT3(  0.0f,  0.0f,  1.0f ), XMFLOAT2(  0.0f, 0.0f )},  // 12
			{ XMFLOAT3(  1.0f,	 1.0f,  1.0f ), XMFLOAT3(  0.0f,  0.0f,  1.0f ), XMFLOAT2(  0.5f,  .0f )},  // 13
			{ XMFLOAT3(  1.0f,	-1.0f,  1.0f ), XMFLOAT3(  0.0f,  0.0f,  1.0f ), XMFLOAT2(  0.5f,  .5f )},  // 14
			{ XMFLOAT3( -1.0f,	-1.0f,  1.0f ), XMFLOAT3(  0.0f,  0.0f,  1.0f ), XMFLOAT2(  0.0f,  .5f )},  // 15

			{ XMFLOAT3( -1.0f,	 1.0f, -1.0f ), XMFLOAT3(  0.0f,  1.0f,  0.0f ), XMFLOAT2(  0.5f, 0.5f )},  // 16
			{ XMFLOAT3(  1.0f,	 1.0f, -1.0f ), XMFLOAT3(  0.0f,  1.0f,  0.0f ), XMFLOAT2(  1.0f, 0.5f )},  // 17
			{ XMFLOAT3( -1.0f,	 1.0f,  1.0f ), XMFLOAT3(  0.0f,  1.0f,  0.0f ), XMFLOAT2(  0.5f, 1.0f )},  // 18
			{ XMFLOAT3(  1.0f,	 1.0f,  1.0f ), XMFLOAT3(  0.0f,  1.0f,  0.0f ), XMFLOAT2(  1.0f, 1.0f )},  // 19

			{ XMFLOAT3( -1.0f,	-1.0f, -1.0f ), XMFLOAT3(  0.0f, -1.0f,  0.0f ), XMFLOAT2(  0.0f, 0.5f )},  // 20
			{ XMFLOAT3(  1.0f,	-1.0f, -1.0f ), XMFLOAT3(  0.0f, -1.0f,  0.0f ), XMFLOAT2(  0.5f, 0.5f )},  // 21
			{ XMFLOAT3( -1.0f,	-1.0f,  1.0f ), XMFLOAT3(  0.0f, -1.0f,  0.0f ), XMFLOAT2(  0.0f, 1.0f )},  // 22
			{ XMFLOAT3(  1.0f,	-1.0f,  1.0f ), XMFLOAT3(  0.0f, -1.0f,  0.0f ), XMFLOAT2(  0.5f, 1.0f )},  // 23

		};

		inline static 	std::vector<UINT> Indices = {
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
		};

		void Init()
		{
			m_psConstants.reflectiveness = 1;
			m_psConstants.reflectionPower = 16;
			Drawable::Init();
		}

		void RegisterResources() {
			mVertexBuffer =		VertexBuffer<VertexTexturedWithNormal>::		Get("cube:texturedWithNormal", Vertices);
			mIndexBuffer =		IndexBuffer::											Get("cube:texturedWithNormal", Indices);
			mInputLayout =		InputLayout::											Get(VertexType::TexturedWithNormal);
			mVsConstantBuffer =	VSConstantBuffer<WorldTransform>::				Create(mVsConstants, ResourceSlots::PerInstance);
			mPsConstantBuffer =	PSConstantBuffer<Specular>::						Create(m_psConstants, ResourceSlots::PerInstance);
			mIndicesCount =		(UINT)Indices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX viewPerspectiveTransform)
		{
			mVsConstants.worldTransform = viewPerspectiveTransform;
			std::static_pointer_cast<VSConstantBuffer<WorldTransform >> (mVsConstantBuffer)->Update(mVsConstants);
		}

	private:
		WorldTransform						mVsConstants = {};
		Specular								m_psConstants = {};
	};
}
