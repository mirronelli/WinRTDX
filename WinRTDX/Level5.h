#pragma once
#include "pch.h"
#include "ILevel.h"
#include "winrt/Windows.Foundation.Numerics.h"
#include "Drawable.h"
#include "VertexShader.h"

using namespace winrt::Windows::Foundation::Numerics;
using namespace Dx::Attachables;

namespace Dx::Levels
{

	class Level5 : public ILevel
	{
	public:
		Level5(std::shared_ptr<Dx::Graphics> graphics) : ILevel(graphics) {};

		concurrency::task<void> Load();
		void SetupModel();
		void DrawCube(float angle, float x, float y, float z);
		void Render();
		void Update(float delta, KeyMap keyMap);

	private:
		std::vector<std::shared_ptr<Drawable>>								m_drawables;
		std::shared_ptr<VertexShader>											m_vertexShaderSimple;
		std::shared_ptr<PixelShader>											m_pixelShaderSimple;
		std::shared_ptr<VertexShader>											m_vertexShaderTextured;
		std::shared_ptr<PixelShader>											m_pixelShaderTextured;
		std::shared_ptr<Texture>												m_texture;

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