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

		typedef struct {
			DirectX::XMMATRIX matrix;
		} VSConstants;

		inline static std::vector<D3D11_INPUT_ELEMENT_DESC> Ieds {
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

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
			m_psConstants = {
					DirectX::XMFLOAT4(1.f, 0.5f, 0.5f, 1.f),
					DirectX::XMFLOAT4(0.5f, 1.f, 0.5f, 1.f),
					DirectX::XMFLOAT4(0.5f, 0.5f, 1.f, 1.f),
					DirectX::XMFLOAT4(1.f, 0.5f, 1.f, 1.f),
					DirectX::XMFLOAT4(0.5f, 1.f, 1.f, 1.f),
					DirectX::XMFLOAT4(1.f, 1.f, 0.5f, 1.f),
			};

			m_vertexBuffer =		VertexBuffer<VertexSimple>::		Get("cube", Vertices);
			m_indexBuffer =		IndexBuffer::							Get("cube", Indices);
			m_psConstantBuffer =	PSConstantBuffer<PSConstants>::	Create(m_psConstants, ResourceSlots::PerInstance);
			m_vsConstantBuffer =	VSConstantBuffer<VSConstants>::	Create(m_vsConstants, ResourceSlots::PerInstance);
			m_inputLayout =		InputLayout::							Get(VertexType::Simple);
			m_indicesCount =		(UINT)Indices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			m_vsConstants.matrix = matrix;			
			std::static_pointer_cast<VSConstantBuffer<VSConstants >> (m_vsConstantBuffer)->Update(m_vsConstants);
		}

	private:
		PSConstants															m_psConstants = {};
		VSConstants															m_vsConstants = {};
	};
}
