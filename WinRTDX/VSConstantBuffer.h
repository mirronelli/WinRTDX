#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"

namespace Dx::Attachables
{
	template<class T>
	class VSConstantBuffer : public Attachable
	{
	public:
		static std::shared_ptr<VSConstantBuffer<T>> Create(T& constantData, ResourceSlots slot = ResourceSlots::PerLevel, bool fastMode = true)
		{
			return std::make_shared<VSConstantBuffer>(constantData, (UINT)slot, fastMode);;
		}

		VSConstantBuffer(T& constantData, UINT slot, bool fastMode)
			: 
			mSlot(slot),
			mFastMode(fastMode)
		{
			D3D11_BUFFER_DESC	desc{ 0 };
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
			desc.Usage = fastMode ? D3D11_USAGE::D3D11_USAGE_DYNAMIC : D3D11_USAGE::D3D11_USAGE_DEFAULT;
			desc.CPUAccessFlags = fastMode ? D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE : 0;
			desc.ByteWidth = sizeof(T);

			D3D11_SUBRESOURCE_DATA srd{ 0 };
			srd.pSysMem = &constantData;

			Graphics::Device->CreateBuffer(&desc, &srd, mBuffer.put());
		}

		void Attach()
		{
			ID3D11Buffer* VSConstantBuffers[1] = { mBuffer.get() };
			Graphics::Context->VSSetConstantBuffers(mSlot, 1, VSConstantBuffers);
		}

		void Update(T const& constantData)
		{
			if (mFastMode)
			{
				D3D11_MAPPED_SUBRESOURCE subresource;
				Graphics::Context->Map(mBuffer.get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subresource);
				memcpy(subresource.pData, &constantData, sizeof(constantData));
				Graphics::Context->Unmap(mBuffer.get(), 0);
			}
			else
				Graphics::Context->UpdateSubresource(mBuffer.get(), 0, 0, &constantData, 0, 0);
		}

	private:
		com_ptr<ID3D11Buffer>	mBuffer;
		UINT							mSlot;
		bool							mFastMode;
	};
}
