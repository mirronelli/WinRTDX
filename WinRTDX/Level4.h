#pragma once
#include "pch.h"
#include "LevelBase.h"
#include "Drawable.h"
#include "VertexShader.h"

using namespace Dx::Attachables;
using namespace Dx::Drawables;

namespace Dx::Levels
{

	class Level4 : public LevelBase
	{
		using LevelBase::LevelBase;

	public:
		concurrency::task<void> Load();
		void SetupModel();
		void DrawCube(float angle, float x, float y, float z);
		void Render();
		void Update(float delta);

	private:
		std::vector<std::shared_ptr<Drawable>>								mDrawables;
		std::shared_ptr<VertexShader>											mVertexShaderSimple;
		std::shared_ptr<PixelShader>											mPixelShaderSimple;

		float																			mWorldRotationX{ 0 };
		float																			mWorldRotationY{ 0 };
		float																			mWorldRotationZ{ 0 };
																						
		float																			mWorldRotationSpeedX{ 0 };
		float																			mWorldRotationSpeedY{ 0 };
		float																			mWorldRotationSpeedZ{ 0 };
																						
		DirectX::XMMATRIX															mWorldViewTransform{};
		std::shared_ptr<VSConstantBuffer<DirectX::XMMATRIX>>			mWorldViewTransformConstantBuffer;
	};
}