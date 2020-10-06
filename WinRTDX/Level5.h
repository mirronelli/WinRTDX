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
		std::shared_ptr<VertexShader>											m_vertexShaderSimple;
		std::shared_ptr<PixelShader>											m_pixelShaderSimple;
		std::shared_ptr<VertexShader>											mVertexShaderTextured;
		std::shared_ptr<PixelShader>											mPixelShaderTextured;
		std::shared_ptr<Texture>												mTexture;

		float																			m_worldRotationX{ 0 };
		float																			m_worldRotationY{ 0 };
		float																			m_worldRotationZ{ 0 };

		float																			m_worldRotationSpeedX{ 0 };
		float																			m_worldRotationSpeedY{ 0 };
		float																			m_worldRotationSpeedZ{ 0 };

		DirectX::XMMATRIX															m_worldViewTransform{};
		std::shared_ptr<VSConstantBuffer<DirectX::XMMATRIX>>			m_worldViewTransformConstantBuffer;
	};
}