#pragma once
#include "pch.h"
#include "ILevel.h"
#include "winrt/Windows.Foundation.Numerics.h"

using namespace winrt::Windows::Foundation::Numerics;

namespace Dx::Levels
{

	class Level3 : public ILevel
	{
		struct Vertex
		{
			float x, y, z, w; // vertex position
		};

	public:
		Level3(std::shared_ptr<Dx::Graphics> graphics) : ILevel(graphics) {};

		concurrency::task<void> Load();
		void SetupModel();
		void Render();
		void Update(float delta);

	private:
		com_ptr<ID3D11Buffer>	m_vertexBuffer;
		com_ptr<ID3D11Buffer>	m_indexBuffer;
		com_ptr<ID3D11Buffer>	m_constantBuffer;
		IBuffer						m_compiledVertexShader{ nullptr };
		IBuffer						m_compiledPixelShader{ nullptr };

		float							m_elapsedTime{ 0 };
		float							m_effectDuration{ 5.f };

		DirectX::XMMATRIX m_matrices = DirectX::XMMatrixRotationZ(0.f);
	};
}