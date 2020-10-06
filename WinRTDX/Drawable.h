#pragma once
#include "pch.h"
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

#define MAKEID(A,B) std::string(A) + std::to_string(B)

namespace Dx::Drawables
{

	class Drawable : public ObjectInSpace
	{
	public:
		Drawable(
			std::shared_ptr<VertexShader> vertexShader,
			std::shared_ptr<PixelShader> pixelShader
		) :
			m_vertexShader(vertexShader),
			m_pixelShader(pixelShader)
		{};

		virtual ~Drawable() {};
		virtual void Init() { RegisterResources(); mInitialized = true; };
		virtual void RegisterResources() = 0;
		virtual void UpdateConstants(DirectX::CXMMATRIX) = 0;

		void AttachResources(bool ){
			if (m_vertexBuffer != nullptr)
				m_vertexBuffer->Attach();

			if (m_indexBuffer != nullptr)
				m_indexBuffer->Attach();

			if (m_inputLayout != nullptr)
				m_inputLayout->Attach();

			if (m_psConstantBuffer != nullptr)
				m_psConstantBuffer->Attach();

			if (m_vsConstantBuffer != nullptr)
				m_vsConstantBuffer->Attach();

			if (m_texture != nullptr)
				m_texture->Attach();

			if (m_vertexShader != nullptr)
				m_vertexShader->Attach();

			if (m_pixelShader != nullptr)
				m_pixelShader->Attach();
		}

		void Draw() {
			assert(mInitialized);

			UpdateConstants(mTransform);
			AttachResources(false);
			Graphics::Context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			Graphics::Context->DrawIndexed(m_indicesCount, 0, 0);
		};

		void Texture(std::shared_ptr<Attachables::Texture> value) { m_texture = value; }

	protected:
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