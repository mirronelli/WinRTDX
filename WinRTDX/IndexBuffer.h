#pragma once
#include "pch.h"
#include "Attachable.h"
#include "Graphics.h"

namespace Dx::Attachables
{
	class IndexBuffer : public Attachable
	{
	public:
		IndexBuffer(std::shared_ptr<Graphics> graphics, std::vector<UINT> const& indices) : Attachable(graphics)
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
		com_ptr<ID3D11Buffer>				m_buffer;
	};
}
