#pragma once
#include "pch.h"
#include "ILevel.h"
#include "winrt/Windows.Foundation.Numerics.h"
#include "VertexBuffer.h"
#include "Drawable.h"

using namespace winrt::Windows::Foundation::Numerics;

namespace Dx::Levels
{

	class Level4 : public ILevel
	{
	public:
		Level4(std::shared_ptr<Dx::Graphics> graphics) : ILevel(graphics) {};

		concurrency::task<void> Load();
		void SetupModel();
		void DrawCube(float angle, float x, float y, float z);
		void Render();
		void Update(float delta);

	private:
		std::vector<std::shared_ptr<Drawable>>	m_drawables;
		com_ptr<ID3D11Buffer>						m_constantBuffer;
		IBuffer											m_compiledVertexShader{ nullptr };
		IBuffer											m_compiledPixelShader{ nullptr };

		float												m_elapsedTime{ 0 };
		float												m_effectDuration{ 5.f };
		float												m_progress{ 0 };

		
	};
}