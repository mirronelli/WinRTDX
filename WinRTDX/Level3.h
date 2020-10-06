#pragma once
#include "pch.h"
#include "ILevel.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace Dx::Levels
{

	class Level3 : public ILevel
	{
		using ILevel::ILevel;
		
		struct VertexSimple
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
		std::shared_ptr<Dx::Attachables::VertexShader>		mVertexShader;
		std::shared_ptr<Dx::Attachables::PixelShader>		mPixelShader;

		com_ptr<ID3D11Buffer>	mVertexBuffer;
		com_ptr<ID3D11Buffer>	mIndexBuffer;
		com_ptr<ID3D11Buffer>	mConstantBuffer;

		float							mElapsedTime{ 0 };
		float							mEffectDuration{ 5.f };
		float							mProgress{ 0 };

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
		} mConstantData;
	};
}