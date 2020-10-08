#pragma once
#include "pch.h"
#include "Attachable.h"

namespace Dx::Attachables
{
	class IndexBuffer : public Attachable
	{
	public:
		static void Reset() { mMap.clear(); }

		static std::shared_ptr<IndexBuffer> Create(std::string key, std::unique_ptr<std::vector<UINT>> indices)
		{
			std::shared_ptr<IndexBuffer> instance = std::make_shared<IndexBuffer>(key, std::move(indices));
			mMap[key] = instance;
			return instance;
		}

		static std::shared_ptr<IndexBuffer> Get(std::string key)
		{
			return mMap[key];
		}

		unsigned int Count()
		{
			return (unsigned int) mIndices->size();
		}

		IndexBuffer(std::string key, std::unique_ptr<std::vector<UINT>> indices)
			: mKey(key)
		{
			mIndices = std::move(indices);

			D3D11_BUFFER_DESC desc = { 0 };
			desc.ByteWidth = static_cast<UINT>(sizeof(UINT) * mIndices->size());
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
			desc.StructureByteStride = sizeof(UINT);

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = mIndices->data();

			Graphics::Device->CreateBuffer(&desc, &srd, mBuffer.put());
		}

		void Attach()
		{
			if (mKey != mCurrentIndexBuffer)
			{
				mCurrentIndexBuffer = mKey;
				Graphics::Context->IASetIndexBuffer(mBuffer.get(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
			}
		}

	private:
		com_ptr<ID3D11Buffer> mBuffer;
		std::unique_ptr<std::vector<UINT>> mIndices;

		std::string mKey;
		inline static std::map < std::string, std::shared_ptr<IndexBuffer>> mMap;
	};
}
