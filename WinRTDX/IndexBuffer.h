#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"


namespace Dx::Attachables
{
	class IndexBuffer : public Attachable
	{
	public:
		static std::shared_ptr<IndexBuffer> Create(uint16_t key, bool overwrite, std::shared_ptr<Graphics> graphics, std::vector<UINT> const& indices)
		{
			std::shared_ptr<IndexBuffer> instance = std::static_pointer_cast<IndexBuffer>( ResourceManager::IndexBuffers[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<IndexBuffer>(key, graphics, indices);
				ResourceManager::IndexBuffers[key] = instance;
			}

			return instance;
		}

		IndexBuffer(uint16_t const& key, std::shared_ptr<Graphics> graphics, std::vector<UINT> const& indices)
			: Attachable(key, graphics)
		{
			D3D11_BUFFER_DESC desc = { 0 };
			desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices.size());
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
			desc.StructureByteStride = sizeof(UINT);

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = indices.data();

			m_device->CreateBuffer(&desc, &srd, m_buffer.put());
		}

		void AttachPrivate(bool force)
		{
			if (force || ResourceManager::CurrentIndexBuffer != m_key)
			{
				m_context->IASetIndexBuffer(m_buffer.get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
				ResourceManager::CurrentIndexBuffer = m_key;
			}
		}

	private:
		com_ptr<ID3D11Buffer> m_buffer;
	};
}
