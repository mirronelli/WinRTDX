#pragma once
#include "pch.h"
#include "ILevel.h"
#include "winrt/Windows.Foundation.Numerics.h"

using namespace winrt::Windows::Foundation::Numerics;

namespace Dx::Levels
{

	class Level3 : public ILevel
	{
		using ILevel::ILevel;
		
		struct Vertex
		{
			float x, y, z, w; // vertex position
		};

	public:
		concurrency::task<void> Load();
		void CreateVertices();
		void CreateIndices();
		void CreateConstantData();
		void RegisterBuffers();
		void SetupModel();
		void DrawCube(float angle, float x, float y, float z);
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
		float							m_progress{ 0 };

		struct ConstantBuffer {
			DirectX::XMMATRIX matrix{ DirectX::XMMatrixRotationZ(0.f) };
			DirectX::XMFLOAT4 colors[6]{
				DirectX::XMFLOAT4(1.f, 0.5f, 0.5f, 1.f),
				DirectX::XMFLOAT4(0.5f, 1.f, 0.5f, 1.f),
				DirectX::XMFLOAT4(0.5f, 0.5f, 1.f, 1.f),
				DirectX::XMFLOAT4(1.f, 0.5f, 1.f, 1.f),
				DirectX::XMFLOAT4(0.5f, 1.f, 1.f, 1.f),
				DirectX::XMFLOAT4(1.f, 1.f, 0.5f, 1.f),
			};
		} m_constantData;
	};
}