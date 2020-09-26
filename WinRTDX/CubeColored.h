#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>

using namespace Dx::Attachables;
using namespace DirectX;
namespace Dx {

	class CubeColored : public Drawable
	{
		using Drawable::Drawable;

	public:
		struct Vertex {
			DirectX::XMFLOAT3	position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT3	color;
		};

		typedef struct {
			DirectX::XMMATRIX matrix;
		} VSConstants;

		inline static std::vector<D3D11_INPUT_ELEMENT_DESC> Ieds{
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		inline static std::vector<Vertex> Vertices  {
			{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT3( 1.0f, 0.0f, 0.0f )},  // 0==============
			{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT3( 0.0f, 1.0f, 0.0f )},  // 1
			{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT3( 0.0f, 0.0f, 1.0f )},  // 2
			{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f,  0.0f, -1.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f )},  // 3

			{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT3( 1.0f, 1.0f, 0.0f )},  // 4==============
			{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT3( 0.0f, 1.0f, 1.0f )},  // 5
			{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT3( 1.0f, 0.0f, 1.0f )},  // 6
			{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(1.0f,  0.0f,  0.0f), XMFLOAT3( 1.0f, 1.0f, 1.0f )},  // 7

			{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f,  0.0f), XMFLOAT3( 1.0f, 0.5f, 0.0f )},  // 8==============
			{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(-1.0f, 0.0f,  0.0f), XMFLOAT3( 0.5f, 1.0f, 0.0f )},  // 9
			{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(-1.0f, 0.0f,  0.0f), XMFLOAT3( 0.5f, 0.0f, 1.0f )},  // 10
			{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f,  0.0f), XMFLOAT3( 1.0f, 0.0f, 0.5f )},  // 11

			{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT3( 0.0f, 1.0f, 0.5f )},  // 12=============
			{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT3( 0.0f, 0.5f, 1.0f )},  // 13
			{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT3( 0.5f, 0.5f, 0.0f )},  // 14
			{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f,  0.0f,  1.0f), XMFLOAT3( 0.0f, 0.5f, 0.5f )},  // 15

			{ XMFLOAT3(-1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3( 0.5f, 0.0f, 0.5f )},  // 16=============
			{ XMFLOAT3(1.0f,	 1.0f, -1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3( 1.0f, 0.5f, 1.0f )},  // 17
			{ XMFLOAT3(-1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3( 1.0f, 1.0f, 0.5f )},  // 18
			{ XMFLOAT3(1.0f,	 1.0f,  1.0f), XMFLOAT3(0.0f,  1.0f,  0.0f), XMFLOAT3( 0.5f, 1.0f, 1.0f )},  // 19

			{ XMFLOAT3(-1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT3( 0.0f, 0.5f, 0.0f )},  // 20=============
			{ XMFLOAT3(1.0f,	-1.0f, -1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT3( 0.5f, 0.5f, 0.0f )},  // 21
			{ XMFLOAT3(-1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT3( 0.0f, 1.0f, 0.0f )},  // 22
			{ XMFLOAT3(1.0f,	-1.0f,  1.0f), XMFLOAT3(0.0f, -1.0f,  0.0f), XMFLOAT3( 0.5f, 1.0f, 0.0f )},  // 23

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

		void RegisterResources() {
			m_vertexBuffer =		VertexBuffer<Vertex>::Create				(m_resourceCacheID, false, m_graphics, Vertices);
			m_indexBuffer =		IndexBuffer::Create							(m_resourceCacheID, false, m_graphics, Indices);
			m_vsConstantBuffer =	VSConstantBuffer<VSConstants>::Create	(m_resourceCacheID, false, m_graphics, m_vsConstants, 2);
			m_inputLayout =		InputLayout::Create							(m_resourceCacheID, false, m_graphics, Ieds, m_vertexShader);
			m_indicesCount =		(UINT)Indices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			m_vsConstants.matrix = matrix;			
			std::static_pointer_cast<VSConstantBuffer<VSConstants >> (m_vsConstantBuffer)->Update(m_vsConstants);
		}

	private:
		VSConstants															m_vsConstants = {};
	};
}
