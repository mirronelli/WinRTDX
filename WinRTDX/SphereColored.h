#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Structures.h"

using namespace Dx::Attachables;
using namespace DirectX;

namespace Dx::Drawables 
{

	class SphereColored : public Dx::Drawables::Drawable
	{
	public:
		SphereColored (
			std::shared_ptr<VertexShader> vertexShader,
			std::shared_ptr<PixelShader> pixelShader,
			int resourceCacheID,
			int steps
		) :
			Drawable(vertexShader, pixelShader, resourceCacheID),
			m_steps(steps)
		{}

		void Init()
		{
			GenerateVerticesAndIndices();
			m_sharedConstants.reflectionPower = 16;
			m_sharedConstants.reflectiveness = 0.5;

			Drawable::Init();
		}

		void Color(XMFLOAT3 value) { m_color = value; m_useRandomColor = false; }
		void ColorRanges(XMFLOAT3 minValue, XMFLOAT3 maxValue) { m_colorMin = minValue; m_colorMax = maxValue; m_useRandomColor = true; }

		struct VertexSimple {
			DirectX::XMFLOAT3	position;
			DirectX::XMFLOAT3 normal;
			DirectX::XMFLOAT3	color;
		};

		typedef struct {
		} PSConstants;

		typedef struct {
			DirectX::XMMATRIX matrix;
			float	reflectiveness;
			float reflectionPower;
			DirectX::XMFLOAT2	padding;
		} SharedConstants;

		inline static std::vector<D3D11_INPUT_ELEMENT_DESC> Ieds{
			{ "POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 0,		D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 12,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR",		0,	DXGI_FORMAT_R32G32B32_FLOAT,	0, 24,	D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		std::vector<VertexSimple> Vertices;
		std::vector<UINT> Indices;
		

		void RegisterResources() {
			m_vertexBuffer =		VertexBuffer<VertexSimple>::				Create(m_resourceCacheID, false, Vertices);
			m_indexBuffer =		IndexBuffer::								Create(m_resourceCacheID, false, Indices);
			m_vsConstantBuffer = VSConstantBuffer<SharedConstants>::	Create(m_resourceCacheID, false, m_sharedConstants, 2);
			m_psConstantBuffer = PSConstantBuffer<SharedConstants>::	Create(m_resourceCacheID, false, m_sharedConstants, 2);
			m_inputLayout =		ResourceManager::GetInputLayout(VertexType::Colored);
			m_indicesCount =		(int)Indices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			m_sharedConstants.matrix = matrix;
			std::static_pointer_cast<VSConstantBuffer<SharedConstants >> (m_vsConstantBuffer)->Update(m_sharedConstants);
			std::static_pointer_cast<PSConstantBuffer<SharedConstants >> (m_psConstantBuffer)->Update(m_sharedConstants);
		}

	private:
		void GenerateVerticesAndIndices()
		{
			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 generator(rd());

			std::uniform_real_distribution<float> randomRed (m_colorMin.x, m_colorMax.x) ;
			std::uniform_real_distribution<float> randomGreen (m_colorMin.y, m_colorMax.y);
			std::uniform_real_distribution<float> randomBlue (m_colorMin.z, m_colorMax.z);
			
			float step = DirectX::XM_PI / m_steps;
			int meridians = m_steps * 2;

			int northPole = 0;
			int southPole = meridians * (m_steps - 1) + 1;
			XMVECTOR vector = XMVectorSet(0, 1, 0, 0);

			int counter = 1;

			// north pole
			Vertices.push_back(
				{ 
					XMFLOAT3( 0, 1, 0 ), 
					XMFLOAT3(0, 1, 0), 
					m_useRandomColor ? XMFLOAT3(randomRed(generator), randomGreen(generator), randomBlue(generator)) : m_color 
				}
			);

			// rotate vector to each parallel
			for (int i = 1; i < m_steps; i++)
			{
				// rotate vector to each meridian of the parallel
				for (int j = 0; j < meridians; j++)
				{	
					XMFLOAT3 vertex;
					XMStoreFloat3(&vertex, XMVector3Rotate(vector, XMQuaternionRotationRollPitchYaw(i * step, j * step, 0)));
					Vertices.push_back(
						{ 
							vertex, 
							vertex, 
							m_useRandomColor ? XMFLOAT3(randomRed(generator), randomGreen(generator), randomBlue(generator)) : m_color 
						}
					);

					// generate triangles of inner vertices within strips
					if (counter > northPole && counter % meridians != 0 && counter < southPole - 1 - meridians)
					{
						Indices.push_back(counter);
						Indices.push_back(counter + meridians);
						Indices.push_back(counter + 1);

						Indices.push_back(counter + 1);
						Indices.push_back(counter + meridians);
						Indices.push_back(counter + meridians + 1);
					}
					counter++;
				}
			}
			// south pole
			Vertices.push_back(
				{ 
					XMFLOAT3( 0, -1, 0 ), 
					XMFLOAT3(0, -1, 0), 
					m_useRandomColor ? XMFLOAT3(randomRed(generator), randomGreen(generator), randomBlue(generator)) : m_color 
				} 
			);

			// join poles with strips
			for (int i = 1; i < meridians; i++)
			{
				Indices.push_back(northPole);  // north pole
				Indices.push_back(northPole + i);
				Indices.push_back(northPole + i + 1);

				Indices.push_back(southPole);  // south pole
				Indices.push_back(southPole - i);
				Indices.push_back(southPole - i - 1);
			}

			// join poles with both strip ends
			Indices.push_back(northPole);
			Indices.push_back(northPole + meridians);
			Indices.push_back(northPole + 1);

			Indices.push_back(southPole);
			Indices.push_back(southPole - meridians);
			Indices.push_back(southPole -1);

			//join strip ends
			for (int i = northPole + 1; i < southPole - 1 - meridians; i += meridians)
			{
				Indices.push_back(i);
				Indices.push_back(i + meridians - 1);
				Indices.push_back(i + meridians);

				Indices.push_back(i + meridians - 1);
				Indices.push_back(i + meridians - 1 + meridians);
				Indices.push_back(i + meridians);
			}
		}

		SharedConstants	m_sharedConstants = {};
		int	m_steps;
		XMFLOAT3 m_color = { 1,1,1 };
		XMFLOAT3 m_colorMin;
		XMFLOAT3 m_colorMax;
		bool m_useRandomColor = false;
	};
}
