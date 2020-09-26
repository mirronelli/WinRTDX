#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include <DirectXMath.h>

using namespace Dx::Attachables;
using namespace DirectX;
namespace Dx {

	class SphereColored : public Drawable
	{
	public:
		SphereColored (
			std::shared_ptr<Graphics> graphics,
			std::shared_ptr<VertexShader> vertexShader,
			std::shared_ptr<PixelShader> pixelShader,
			uint16_t resourceCacheID,
			uint16_t steps,
			XMFLOAT3 color
		) :
			Drawable(graphics, vertexShader, pixelShader, resourceCacheID),
			m_steps(steps),
			m_color(color)
		{
			GenerateVertices();
			GenerateIndices();
		}

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

		std::vector<Vertex> Vertices;
		std::vector<UINT> Indices;
		

		void RegisterResources() {
			m_vertexBuffer =		VertexBuffer<Vertex>::				Create(m_resourceCacheID, false, m_graphics, Vertices);
			m_indexBuffer =		IndexBuffer::							Create(m_resourceCacheID, false, m_graphics, Indices);
			m_vsConstantBuffer = VSConstantBuffer<VSConstants>::	Create(m_resourceCacheID, false, m_graphics, m_vsConstants, 1);
			m_inputLayout =		InputLayout::							Create(m_resourceCacheID, false, m_graphics, Ieds, m_vertexShader);
			m_indicesCount =		(UINT)Indices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			m_vsConstants.matrix = matrix;
			std::static_pointer_cast<VSConstantBuffer<VSConstants >> (m_vsConstantBuffer)->Update(m_vsConstants);
		}

	private:
		void GenerateVertices()
		{
			float step = DirectX::XM_PI / m_steps;
			XMVECTOR vector = XMVectorSet(0, 1, 0, 0);

			// north pole
			Vertices.push_back({ XMFLOAT3( 0, 1, 0 ), XMFLOAT3(0, 1, 0), m_color });

			for (int i = 1; i < m_steps; i++)
			{
				for (int j = 0; j < m_steps * 2; j++)
				{	
					XMFLOAT3 vertex;
					XMStoreFloat3(&vertex, XMVector3Rotate(vector, XMQuaternionRotationRollPitchYaw(i * step, j * step, 0)));
					Vertices.push_back({ vertex, vertex, m_color });
				}
			}

			// south pole
			Vertices.push_back({ XMFLOAT3( 0, -1, 0 ), XMFLOAT3(0, -1, 0), m_color });
		}

		void GenerateIndices()
		{
			int meridians = m_steps * 2;
			int verticesCount = Vertices.size();

			// north pole to start and end in first line
			Indices.push_back(0);
			Indices.push_back(meridians);
			Indices.push_back(1);

			// north pole to each in first line
			for (int i = 1; i < m_steps * 2; i++)
			{
				Indices.push_back(0);
				Indices.push_back(i);
				Indices.push_back(i+1);
			}

			//strips
			for (int strip = 0; strip < m_steps - 2; strip++)
			{
				int stripStart = strip * meridians;
				
				//strip from start to finis
				for (int i = 1; i < m_steps * 2; i++)
				{
					Indices.push_back(stripStart + i);
					Indices.push_back(stripStart + i + meridians);
					Indices.push_back(stripStart + i + 1);

					Indices.push_back(stripStart + i + 1);
					Indices.push_back(stripStart + i + meridians);
					Indices.push_back(stripStart + i + meridians + 1);
				}

				//strip from end to start
				Indices.push_back(stripStart + 1);
				Indices.push_back(stripStart + 1 + 2*meridians - 1);
				Indices.push_back(stripStart + 1 + meridians);

				Indices.push_back(stripStart + 1);
				Indices.push_back(stripStart + 1 + meridians - 1);
				Indices.push_back(stripStart + 1 + 2*meridians - 1);
			}

			// south pole to start and end in last line
			Indices.push_back(verticesCount - 1);
			Indices.push_back(verticesCount - 1 - meridians);
			Indices.push_back(verticesCount - 1 -1);

			// south pole to each in last line
			for (int i = 0; i < meridians; i++)
			{
				Indices.push_back(verticesCount -1);
				Indices.push_back(verticesCount - i - 2);
				Indices.push_back(verticesCount - i - 3);
			}

		}

		VSConstants	m_vsConstants = {};
		uint16_t	m_steps;
		XMFLOAT3 m_color;
	};
}
