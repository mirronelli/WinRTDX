#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Structures.h"
#include "Cache.h"

using namespace Dx::Attachables;
using namespace DirectX;

namespace Dx::Drawables
{

	class SphereColored: public Dx::Drawables::Drawable
	{
	public:
		SphereColored(
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
			Drawable::Init();
		}

		void Color(XMFLOAT3 value)
		{
			m_color = value;
			m_useRandomColor = false;
		}

		void ColorRanges(XMFLOAT3 minValue, XMFLOAT3 maxValue)
		{
			m_colorMin = minValue;
			m_colorMax = maxValue;
			m_useRandomColor = true;
		}

		void RegisterResources() {
			m_vertexBuffer = VertexBuffer<VertexColored>::Create(m_resourceCacheID, false, mVertices);
			m_indexBuffer = IndexBuffer::Create(m_resourceCacheID, false, mIndices);
			m_vsConstantBuffer = VSConstantBuffer<WorldTransform>::Create(m_resourceCacheID, false, mVertexPerInstanceConstants, (UINT)ResourceSlots::PerInstance);
			m_inputLayout = Dx::Attachables::Cache<InputLayout>::Get(VertexType::Colored);
			m_indicesCount = (int)mIndices.size();
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			mVertexPerInstanceConstants.worldTransform = matrix;
			std::static_pointer_cast<VSConstantBuffer<WorldTransform >> (m_vsConstantBuffer)->Update(mVertexPerInstanceConstants);
		}

	private:
		void GenerateVerticesAndIndices()
		{
			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 generator(rd());

			std::uniform_real_distribution<float> randomRed(m_colorMin.x, m_colorMax.x);
			std::uniform_real_distribution<float> randomGreen(m_colorMin.y, m_colorMax.y);
			std::uniform_real_distribution<float> randomBlue(m_colorMin.z, m_colorMax.z);

			float step = DirectX::XM_PI / m_steps;
			int meridians = m_steps * 2;

			int northPole = 0;
			int southPole = meridians * (m_steps - 1) + 1;
			XMVECTOR vector = XMVectorSet(0, 1, 0, 0);

			int counter = 1;

			// north pole
			mVertices.push_back(
				{
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
					mVertices.push_back(
						{
							vertex,
							m_useRandomColor ? XMFLOAT3(randomRed(generator), randomGreen(generator), randomBlue(generator)) : m_color
						}
					);

					// generate triangles of inner vertices within strips
					if (counter > northPole && counter % meridians != 0 && counter < southPole - 1 - meridians)
					{
						mIndices.push_back(counter);
						mIndices.push_back(counter + meridians);
						mIndices.push_back(counter + 1);

						mIndices.push_back(counter + 1);
						mIndices.push_back(counter + meridians);
						mIndices.push_back(counter + meridians + 1);
					}
					counter++;
				}
			}
			// south pole
			mVertices.push_back(
				{
					XMFLOAT3(0, -1, 0),
					m_useRandomColor ? XMFLOAT3(randomRed(generator), randomGreen(generator), randomBlue(generator)) : m_color
				}
			);

			// join poles with strips
			for (int i = 1; i < meridians; i++)
			{
				mIndices.push_back(northPole);  // north pole
				mIndices.push_back(northPole + i);
				mIndices.push_back(northPole + i + 1);

				mIndices.push_back(southPole);  // south pole
				mIndices.push_back(southPole - i);
				mIndices.push_back(southPole - i - 1);
			}

			// join poles with both strip ends
			mIndices.push_back(northPole);
			mIndices.push_back(northPole + meridians);
			mIndices.push_back(northPole + 1);

			mIndices.push_back(southPole);
			mIndices.push_back(southPole - meridians);
			mIndices.push_back(southPole - 1);

			//join strip ends
			for (int i = northPole + 1; i < southPole - 1 - meridians; i += meridians)
			{
				mIndices.push_back(i);
				mIndices.push_back(i + meridians - 1);
				mIndices.push_back(i + meridians);

				mIndices.push_back(i + meridians - 1);
				mIndices.push_back(i + meridians - 1 + meridians);
				mIndices.push_back(i + meridians);
			}
		}

		int	m_steps;
		XMFLOAT3 m_color = { 1,1,1 };
		XMFLOAT3 m_colorMin;
		XMFLOAT3 m_colorMax;
		bool m_useRandomColor = false;

		WorldTransform					mVertexPerInstanceConstants;

		std::vector<VertexColored>	mVertices;
		std::vector<UINT>				mIndices;
	};
}
