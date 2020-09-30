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
#include "ObjectInSpace.h"

using namespace Dx::Attachables;

namespace Dx::Drawables
{
	enum class DrawableTypes
	{
		SphereColored = 1,
		CubeColored = 2,
		CubeTextured = 3,
		Mesh = 4,
	};

	class Drawable : public ObjectInSpace
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
		virtual void Init() { RegisterResources(); mInitialized = true; };
		virtual void RegisterResources() = 0;
		virtual void UpdateConstants(DirectX::CXMMATRIX) = 0;

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
			UpdateConstants(mTransform);
			AttachResources(false);
			m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			m_context->DrawIndexed(m_indicesCount, 0, 0);
		};

#pragma region Property Setters
		void Texture(std::shared_ptr<Attachables::Texture> value) { m_texture = value; }
#pragma endregion

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
		bool mInitialized = false;
	};
}