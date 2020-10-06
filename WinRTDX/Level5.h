#pragma once
#include "pch.h"
#include "ILevel.h"
#include "Drawable.h"
#include "VertexShader.h"

using namespace Dx::Attachables;
using namespace Dx::Drawables;

namespace Dx::Levels
{

	class Level5 : public ILevel
	{
	public:
		using ILevel::ILevel;
		concurrency::task<void> Load();
		void SetupModel();
		void Render();
		void Update(float delta);

	private:
		std::vector<std::shared_ptr<Drawable>>								mDrawables;
		std::shared_ptr<VertexShader>											mVertexShaderSimple;
		std::shared_ptr<PixelShader>											mPixelShaderSimple;
		std::shared_ptr<VertexShader>											mVertexShaderTextured;
		std::shared_ptr<PixelShader>											mPixelShaderTextured;
		std::shared_ptr<Texture>												mTexture;

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