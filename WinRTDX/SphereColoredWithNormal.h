#pragma once
#include "Drawable.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Structures.h"

using namespace Dx::Attachables;
using namespace DirectX;

namespace Dx::Drawables 
{

	class SphereColoredWithNormal : public Dx::Drawables::Drawable
	{
	public:
		SphereColoredWithNormal (
			int steps
		) : 
			Drawable(
				VertexShader::Get(VertexType::ColoredWithNormal),
				PixelShader::Get(VertexType::ColoredWithNormal)
			),
			mSteps(steps)
		{}

		void Color(XMFLOAT3 value) 
		{
			mColor = value; 
			mUseRandomColor = false; 
		}

		void ColorRanges(XMFLOAT3 minValue, XMFLOAT3 maxValue) 
		{ 
			mColorMin = minValue; 
			mColorMax = maxValue; 
			mUseRandomColor = true; 
		}

		void Specular(float reflectiveness, float reflectivePower)
		{
			mPixelPerInstanceConstants.reflectiveness = reflectiveness;
			mPixelPerInstanceConstants.reflectionPower = reflectivePower;
		}

		void RegisterResources() {
			mPixelPerInstanceConstants.reflectionPower = 160.0f;
			mPixelPerInstanceConstants.reflectiveness = 1.f;
			std::string uuid = MAKEID("sphere:coloredWithNormal:", mSteps);

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
			mInputLayout =		InputLayout::											Get(VertexType::ColoredWithNormal);
			mVsConstantBuffer = VSConstantBuffer<WorldTransform>::				Create(mVertexPerInstanceConstants, ResourceSlots::PerInstance);
			mPsConstantBuffer = PSConstantBuffer<Dx::Drawables::Specular>::	Create(mPixelPerInstanceConstants, ResourceSlots::PerInstance);
		}

		void UpdateConstants(DirectX::CXMMATRIX matrix)
		{
			mVertexPerInstanceConstants.worldTransform = matrix;
			std::static_pointer_cast<VSConstantBuffer<WorldTransform >> (mVsConstantBuffer)->Update(mVertexPerInstanceConstants);
			std::static_pointer_cast<PSConstantBuffer<Dx::Drawables::Specular>> (mPsConstantBuffer)->Update(mPixelPerInstanceConstants);
		}

	private:
		void GenerateVerticesAndIndices(std::unique_ptr<std::vector<VertexColoredWithNormal>>& vertices, std::unique_ptr < std::vector< unsigned int> >& indices)
		{
			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::mt19937 generator(rd());

			std::uniform_real_distribution<float> randomRed (mColorMin.x, mColorMax.x) ;
			std::uniform_real_distribution<float> randomGreen (mColorMin.y, mColorMax.y);
			std::uniform_real_distribution<float> randomBlue (mColorMin.z, mColorMax.z);
			
			float step = DirectX::XM_PI / mSteps;
			int meridians = mSteps * 2;

			int northPole = 0;
			int southPole = meridians * (mSteps - 1) + 1;
			XMVECTOR vector = XMVectorSet(0, 1, 0, 0);

			int counter = 1;

			// north pole
			vertices->push_back(
				{ 
					XMFLOAT3( 0, 1, 0 ), 
					XMFLOAT3(0, 1, 0), 
					mUseRandomColor ? XMFLOAT3(randomRed(generator), randomGreen(generator), randomBlue(generator)) : mColor 
				}
			);

			// rotate vector to each parallel
			for (int i = 1; i < mSteps; i++)
			{
				// rotate vector to each meridian of the parallel
				for (int j = 0; j < meridians; j++)
				{	
					XMFLOAT3 vertex;
					XMStoreFloat3(&vertex, XMVector3Rotate(vector, XMQuaternionRotationRollPitchYaw(i * step, j * step, 0)));
					vertices->push_back(
						{ 
							vertex, 
							vertex, 
							mUseRandomColor ? XMFLOAT3(randomRed(generator), randomGreen(generator), randomBlue(generator)) : mColor 
						}
					);

					// generate triangles of inner vertices within strips
					if (counter > northPole && counter % meridians != 0 && counter < southPole - 1 - meridians)
					{
						indices->push_back(counter);
						indices->push_back(counter + meridians);
						indices->push_back(counter + 1);

						indices->push_back(counter + 1);
						indices->push_back(counter + meridians);
						indices->push_back(counter + meridians + 1);
					}
					counter++;
				}
			}
			// south pole
			vertices->push_back(
				{ 
					XMFLOAT3( 0, -1, 0 ), 
					XMFLOAT3(0, -1, 0), 
					mUseRandomColor ? XMFLOAT3(randomRed(generator), randomGreen(generator), randomBlue(generator)) : mColor 
				} 
			);

			// join poles with strips
			for (int i = 1; i < meridians; i++)
			{
				indices->push_back(northPole);  // north pole
				indices->push_back(northPole + i);
				indices->push_back(northPole + i + 1);

				indices->push_back(southPole);  // south pole
				indices->push_back(southPole - i);
				indices->push_back(southPole - i - 1);
			}

			// join poles with both strip ends
			indices->push_back(northPole);
			indices->push_back(northPole + meridians);
			indices->push_back(northPole + 1);
			
			indices->push_back(southPole);
			indices->push_back(southPole - meridians);
			indices->push_back(southPole -1);

			//join strip ends
			for (int i = northPole + 1; i < southPole - 1 - meridians; i += meridians)
			{
				indices->push_back(i);
				indices->push_back(i + meridians - 1);
				indices->push_back(i + meridians);
				
				indices->push_back(i + meridians - 1);
				indices->push_back(i + meridians - 1 + meridians);
				indices->push_back(i + meridians);
			}
		}

		int	mSteps;
		XMFLOAT3 mColor = { 1,1,1 };
		XMFLOAT3 mColorMin;
		XMFLOAT3 mColorMax;
		bool mUseRandomColor = false;

		Dx::Drawables::Specular				mPixelPerInstanceConstants;
		WorldTransform							mVertexPerInstanceConstants;
	};
}
