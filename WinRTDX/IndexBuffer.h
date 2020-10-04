#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include "ResourceManager.h"

namespace Dx::Attachables
{
	class IndexBuffer : public Attachable
	{
	public:
		static std::shared_ptr<IndexBuffer> Create(int key, bool overwrite, std::vector<UINT> const& indices)
		{
			std::shared_ptr<IndexBuffer> instance = std::static_pointer_cast<IndexBuffer>( ResourceManager::IndexBuffers[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<IndexBuffer>(key, indices);
				ResourceManager::IndexBuffers[key] = instance;
			}

			return instance;
		}

		IndexBuffer(UINT const& key, std::vector<UINT> const& indices)
			: Attachable(key)
		{
			D3D11_BUFFER_DESC desc = { 0 };
			desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * indices.size());
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
			desc.StructureByteStride = sizeof(UINT);

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = indices.data();

			Graphics::Device->CreateBuffer(&desc, &srd, m_buffer.put());
		}

		void AttachPrivate(bool force)
		{
			if (force || ResourceManager::CurrentIndexBuffer != m_key)
			{
				Graphics::Context->IASetIndexBuffer(m_buffer.get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
				ResourceManager::CurrentIndexBuffer = m_key;
			}
		}

	private:
		com_ptr<ID3D11Buffer> m_buffer;
	};
}
