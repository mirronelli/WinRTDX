#pragma once
#include "pch.h"
#include "Attachable.h"

namespace Dx::Attachables
{
	template<class T>
	class VertexBuffer : public Attachable
	{
	public:
		static void Reset() { mMap.clear(); }
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
			: mSlot(slot), mKey(key)
		{
			D3D11_BUFFER_DESC desc = { 0 };
			desc.ByteWidth = static_cast<UINT>(sizeof(T) * vertices.size());
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
			desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = vertices.data();

			Graphics::Device->CreateBuffer(&desc, &srd, mBuffer.put());
		}

		void Attach()
		{
			if (mKey != mCurrentVertexBuffer)
			{
				mCurrentVertexBuffer = mKey;
				UINT strideVertices = sizeof(T);
				UINT offsetVertices = 0;
				ID3D11Buffer* vertexBuffers[1] = { mBuffer.get() };
				Graphics::Context->IASetVertexBuffers(mSlot, 1, vertexBuffers, &strideVertices, &offsetVertices);
			}
		}

	private:
		com_ptr<ID3D11Buffer>	mBuffer;
		UINT							mSlot;

		std::string mKey;
		inline static std::map < std::string, std::shared_ptr<VertexBuffer<T>>> mMap;
	};
}
