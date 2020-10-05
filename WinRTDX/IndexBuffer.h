#pragma once
#include "pch.h"
#include "Attachable.h"

namespace Dx::Attachables
{
	class IndexBuffer : public Attachable
	{
	public:
		static std::shared_ptr<IndexBuffer> Get(std::string key, std::vector<UINT>& indices)
		{
			std::shared_ptr<IndexBuffer> instance = mMap[key];
			if (!instance)
			{
				instance = std::make_shared<IndexBuffer>(key, indices);
				mMap[key] = instance;
			}

			return instance;
		}

		IndexBuffer(std::string key, std::vector<UINT> const& indices)
			: mKey(key)
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
			if (force || mKey != mCurrentIndexBuffer)
			{
				mCurrentIndexBuffer = mKey;
				Graphics::Context->IASetIndexBuffer(m_buffer.get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
			}
		}

	private:
		com_ptr<ID3D11Buffer> m_buffer;

		std::string mKey;
		inline static std::map < std::string, std::shared_ptr<IndexBuffer>> mMap;
	};
}
