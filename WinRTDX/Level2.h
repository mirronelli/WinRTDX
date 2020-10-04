#pragma once
#include "pch.h"
#include "ILevel.h"
#include "VertexShader.h"
#include "PixelShader.h"


namespace Dx::Levels
{

	class Level2 : public ILevel
	{
		using ILevel::ILevel;

		struct VertexSimple
		{
			float x, y, z, w; // vertex position
			float r, g, b, a; // vertex color
		};

	public:

		concurrency::task<void> Load();
		void CreateVertices();
		void CreateIndices();
		void CreateConstantData();
		void RegisterBuffers();
		void SetupModel();
		void Draw(float angle, float x, float y, float z);
		void Render();
		void Update(float delta);

	private:
		std::shared_ptr<Dx::Attachables::VertexShader>		m_vertexShader;
		std::shared_ptr<Dx::Attachables::PixelShader>		m_pixelShader;

		com_ptr<ID3D11Buffer>	m_vertexBuffer;
		com_ptr<ID3D11Buffer>	m_indexBuffer;
		com_ptr<ID3D11Buffer>	m_constantBuffer;

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