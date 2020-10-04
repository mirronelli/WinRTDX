#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include <map>
#include "ResourceManager.h"
#include <typeindex>

namespace Dx::Attachables
{
	template<class T>
	class VertexBuffer : public Attachable
	{
	public:
		static std::shared_ptr<VertexBuffer<T>> Get(std::string key, std::vector<T>& vertices, UINT slot = 0)
		{
			std::shared_ptr<VertexBuffer<T>> instance = mMap[key];
			if (!instance)
			{
				instance = std::make_shared<VertexBuffer<T>>(key, vertices, slot);
				mMap[key] = instance;
			}

			return instance;
		}

		VertexBuffer(std::string key, std::vector<T> const& vertices, UINT slot)
			: m_slot(slot), mKey(key)
		{
			D3D11_BUFFER_DESC desc = { 0 };
			desc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = vertices.data();

			Graphics::Device->CreateBuffer(&desc, &srd, m_buffer.put());
		}

		void AttachPrivate(bool force)
		{
			if (force || mKey != mCurrentVertexBuffer)
			{
				mCurrentVertexBuffer = mKey;
				UINT strideVertices = sizeof(T);
				UINT offsetVertices = 0;
				ID3D11Buffer* vertexBuffers[1] = { m_buffer.get() };
				Graphics::Context->IASetVertexBuffers(m_slot, 1, vertexBuffers, &strideVertices, &offsetVertices);
			}
		}

	private:
		com_ptr<ID3D11Buffer>	m_buffer;
		UINT							m_slot;

		std::string mKey;
		inline static std::map < std::string, std::shared_ptr<VertexBuffer<T>>> mMap;
	};
}
