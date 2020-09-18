#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include <map>

namespace Dx::Attachables
{
	class IndexBuffer : public Attachable
	{
	public:
		static std::shared_ptr<IndexBuffer> Create(std::string const& key, bool overwrite, std::shared_ptr<Graphics> graphics, std::vector<UINT> const& indices)
		{
			std::shared_ptr<IndexBuffer> instance = m_instances[key];

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<IndexBuffer>(graphics, indices);
				m_instances[key] = instance;
			}

			return instance;
		}

		IndexBuffer(std::shared_ptr<Graphics> graphics, std::vector<UINT> const& indices)
			: Attachable(graphics)
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

		void AttachPrivate()
		{
			m_context->IASetIndexBuffer(m_buffer.get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
		}

	private:
		inline static std::map<std::string, std::shared_ptr<IndexBuffer>> m_instances = {};
		com_ptr<ID3D11Buffer> m_buffer;
	};
}
