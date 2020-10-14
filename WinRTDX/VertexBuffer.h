#pragma once
#include "pch.h"
#include "Attachable.h"

namespace Dx::Attachables
{
	class VertexBufferBase : public Attachable
	{
	public:
		VertexBufferBase(std::string key, unsigned int slot, unsigned int strideSize) : mSlot(slot), mKey(key), mStrideSize(strideSize) {}

		static void Reset() { mMap.clear(); }

		void Attach()
		{
			if (mKey != mCurrentVertexBuffer)
			{
				mCurrentVertexBuffer = mKey;
				UINT strideVertices = mStrideSize;
				UINT offsetVertices = 0;
				ID3D11Buffer* vertexBuffers[1] = { mBuffer.get() };
				Graphics::Context->IASetVertexBuffers(mSlot, 1, vertexBuffers, &strideVertices, &offsetVertices);
			}
		}
	
		static std::shared_ptr<VertexBufferBase> Get(std::string key)
		{
			return mMap[key];
		}

		UINT							mSlot;
		std::string					mKey;

	protected:
		com_ptr<ID3D11Buffer>	mBuffer;
		UINT							mStrideSize;

		inline static std::map < std::string, std::shared_ptr<VertexBufferBase>> mMap;
	};

	template <typename T>
	class VertexBuffer : public VertexBufferBase
	{
	public:
		static std::shared_ptr<VertexBuffer<T>> Create(std::string key, std::unique_ptr<std::vector<T>> vertices, UINT slot = 0)
		{
			std::shared_ptr<VertexBuffer<T>> instance = std::make_shared<VertexBuffer<T>>(key, std::move(vertices), slot);
			mMap[key] = instance;
			return instance;
		}

		VertexBuffer(std::string key, std::unique_ptr<std::vector<T>> vertices, UINT slot)
			: VertexBufferBase(key, slot, sizeof(T))
		{
			mStrideSize = sizeof(T);
			mVertices = std::move(vertices);

			D3D11_BUFFER_DESC desc = { 0 };
			desc.ByteWidth = static_cast<UINT>(sizeof(T) * mVertices->size());
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = mVertices->data();

			Graphics::Device->CreateBuffer(&desc, &srd, mBuffer.put());
		}

	private:
		std::unique_ptr<std::vector<T>> mVertices;
	};
}
