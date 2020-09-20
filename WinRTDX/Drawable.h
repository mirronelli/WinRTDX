#pragma once
#include <pch.h>
#include "Graphics.h"
#include "InputLayout.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PSConstantBuffer.h"
#include "VSConstantBuffer.h"

using namespace Dx::Attachables;
namespace Dx {

	class Drawable
	{
	public:
		Drawable(
			std::shared_ptr<Graphics> graphics,
			std::shared_ptr<VertexShader> vertexShader,
			std::shared_ptr<PixelShader> pixelShader,

			float x = 0, float y = 0, float z = 0,
			float speedX = 0, float speedY = 0, float speedZ = 0,
			float rotationX = 0, float rotationY = 0, float rotationZ = 0,
			float rotationSpeedX = 0, float rotationSpeedY = 0, float rotationSpeedZ = 0,
			float scaleX = 1, float scaleY = 1, float scaleZ = 1
		) :
			m_graphics(graphics), 
			m_device(graphics->Device()), 
			m_context(graphics->Context()),
			m_vertexShader(vertexShader),
			m_pixelShader(pixelShader),

			m_worldX(x), m_worldY(y), m_worldZ(z),
			m_speedX(speedX), m_speedY(speedY), m_speedZ(speedZ),
			m_rotationX(rotationX), m_rotationY(rotationY), m_rotationZ(rotationZ),
			m_rotationSpeedX(rotationSpeedX), m_rotationSpeedY(rotationSpeedY), m_rotationSpeedZ(rotationSpeedZ),
			m_scaleX(scaleX), m_scaleY(scaleY), m_scaleZ(scaleZ),

			m_indicesCount(0),
			m_transformToWorld(DirectX::XMMatrixIdentity())
		{};
		virtual ~Drawable() {};

		virtual void RegisterResources() = 0;
		virtual void UpdateConstants(DirectX::CXMMATRIX) = 0;

		virtual void Update(float delta) {
			m_rotationX = fmod(m_rotationX + delta * m_rotationSpeedX * DirectX::XM_2PI, DirectX::XM_2PI);
			m_rotationY = fmod(m_rotationY + delta * m_rotationSpeedY * DirectX::XM_2PI, DirectX::XM_2PI);
			m_rotationZ = fmod(m_rotationZ + delta * m_rotationSpeedZ * DirectX::XM_2PI, DirectX::XM_2PI);

			m_worldX += m_speedX * delta;
			m_worldY += m_speedY * delta;
			m_worldZ += m_speedZ * delta;

			m_transformToWorld = 
				DirectX::XMMatrixScaling(m_scaleX, m_scaleY, m_scaleX)

				// rotate object
				* DirectX::XMMatrixRotationZ(m_rotationZ)
				* DirectX::XMMatrixRotationX(m_rotationX)
				* DirectX::XMMatrixRotationY(m_rotationY)

				// move object to its world coords
				* DirectX::XMMatrixTranslation(m_worldX, m_worldY, m_worldZ);
		};

		void AttachResources(bool force) {
			if (m_vertexBuffer != nullptr)
				m_vertexBuffer->Attach(force);

			if (m_indexBuffer != nullptr)
				m_indexBuffer->Attach(force);

			if (m_inputLayout != nullptr)
				m_inputLayout->Attach(force);

			if (m_psConstantBuffer != nullptr)
				m_psConstantBuffer->Attach(force);

			if (m_vsConstantBuffer != nullptr)
				m_vsConstantBuffer->Attach(force);
		}

		void Draw() {
			UpdateConstants(m_transformToWorld);
			AttachResources(false);
			m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_context->DrawIndexed(m_indicesCount, 0, 0);
		};

		void DrawAt(float delta, float x, float y, float z) {
			m_worldX = x;
			m_worldY = y;
			m_worldZ = z;

			Update(delta);
			Draw();
		}

	protected:
		std::shared_ptr<Graphics>						m_graphics;
		com_ptr<ID3D11Device3>							m_device;
		com_ptr<ID3D11DeviceContext4>					m_context;

		std::shared_ptr<PixelShader>					m_pixelShader;
		std::shared_ptr<VertexShader>					m_vertexShader;
		std::shared_ptr<InputLayout>					m_inputLayout;
		std::shared_ptr<IndexBuffer>					m_indexBuffer;

		std::shared_ptr<Attachable>					m_vertexBuffer;
		std::shared_ptr<Attachable>					m_psConstantBuffer;
		std::shared_ptr<Attachable>					m_vsConstantBuffer;

		UINT													m_indicesCount;
		// state
		float		m_worldX;
		float		m_worldY;
		float		m_worldZ;

		float		m_speedX;
		float		m_speedY;
		float		m_speedZ;

		float		m_rotationSpeedX;
		float		m_rotationSpeedY;
		float		m_rotationSpeedZ;

		float		m_rotationX;
		float		m_rotationY;
		float		m_rotationZ;

		float		m_scaleX;
		float		m_scaleY;
		float		m_scaleZ;
		DirectX::XMMATRIX m_transformToWorld{};
	};
}