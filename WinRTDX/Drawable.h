#pragma once
#include "pch.h"
#include "Graphics.h"
#include "InputLayout.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "PSConstantBuffer.h"
#include "VSConstantBuffer.h"
#include "Texture.h"

using namespace Dx::Attachables;
namespace Dx {

	class Drawable
	{
	public:
		Drawable(
			std::shared_ptr<Graphics> graphics,
			std::shared_ptr<VertexShader> vertexShader,
			std::shared_ptr<PixelShader> pixelShader, 
			int resourceCacheID
		) :
			m_graphics(graphics), 
			m_device(graphics->Device()), 
			m_context(graphics->Context()),
			m_vertexShader(vertexShader),
			m_pixelShader(pixelShader),
			m_resourceCacheID(resourceCacheID)
		{};
		virtual ~Drawable() {};
		virtual void Prepare() { m_prepared = true; };
		virtual void RegisterResources() = 0;
		virtual void UpdateConstants(DirectX::CXMMATRIX) = 0;

		virtual void Update(float delta) {
			m_rotationX = fmod(m_rotationX + delta * m_rotationSpeedX * DirectX::XM_2PI, DirectX::XM_2PI);
			m_rotationY = fmod(m_rotationY + delta * m_rotationSpeedY * DirectX::XM_2PI, DirectX::XM_2PI);
			m_rotationZ = fmod(m_rotationZ + delta * m_rotationSpeedZ * DirectX::XM_2PI, DirectX::XM_2PI);

			m_worldX += m_speedX * delta;
			m_worldY += m_speedY * delta;
			m_worldZ += m_speedZ * delta;

			m_worldTransform = 
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

			if (m_texture != nullptr)
				m_texture->Attach(force);

			if (m_vertexShader != nullptr)
				m_vertexShader->Attach(force);

			if (m_pixelShader != nullptr)
				m_pixelShader->Attach(force);
		}

		void Draw() {
			UpdateConstants(m_worldTransform);
			AttachResources(false);
			m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_context->DrawIndexed(m_indicesCount, 0, 0);
		};

		void Texture(std::shared_ptr<Attachables::Texture> value) { m_texture = value; }
		
		void WorldX(float value) { m_worldX = value; }
		void WorldY(float value) { m_worldY = value; }
		void WorldZ(float value) { m_worldZ = value; }

		void SpeedX(float value) { m_speedX = value; }
		void SpeedY(float value) { m_speedY = value; }
		void SpeedZ(float value) { m_speedZ = value; }
		
		void RotationSpeedX(float value) { m_rotationSpeedX = value; }
		void RotationSpeedY(float value) { m_rotationSpeedY = value; }
		void RotationSpeedZ(float value) { m_rotationSpeedZ = value; }
		
		void RotationX(float value) { m_rotationX = value; }
		void RotationY(float value) { m_rotationY = value; }
		void RotationZ(float value) { m_rotationZ = value; }
		
		void ScaleX(float value) { m_scaleX = value; }
		void ScaleY(float value) { m_scaleY = value; }
		void ScaleZ(float value) { m_scaleZ = value; }

	protected:
		std::shared_ptr<Graphics>						m_graphics;
		com_ptr<ID3D11Device3>							m_device;
		com_ptr<ID3D11DeviceContext4>					m_context;

		int													m_resourceCacheID;
		std::shared_ptr<PixelShader>					m_pixelShader;
		std::shared_ptr<VertexShader>					m_vertexShader;
		std::shared_ptr<Attachable>					m_vertexBuffer;
		std::shared_ptr<IndexBuffer>					m_indexBuffer;
		std::shared_ptr<InputLayout>					m_inputLayout;

		std::shared_ptr<Attachables::Texture>		m_texture;

		std::shared_ptr<Attachable>					m_psConstantBuffer;
		std::shared_ptr<Attachable>					m_vsConstantBuffer;

		UINT m_indicesCount = 0;
		DirectX::XMMATRIX m_worldTransform;

		float	m_worldX	= 0; 
		float	m_worldY = 0;
		float	m_worldZ = 0;

		float	m_speedX	= 0;
		float	m_speedY	= 0;
		float	m_speedZ	= 0;

		float	m_rotationSpeedX = 0;
		float	m_rotationSpeedY = 0;
		float	m_rotationSpeedZ = 0;

		float	m_rotationX	= 0;
		float	m_rotationY	= 0;
		float	m_rotationZ	= 0;

		float	m_scaleX	= 1;
		float	m_scaleY	= 1;
		float	m_scaleZ	= 1;

		bool m_prepared = false;
	};
}