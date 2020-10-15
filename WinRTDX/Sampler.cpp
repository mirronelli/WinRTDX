#include "pch.h"
#include "Sampler.h"

namespace Dx::Attachables
{
	std::shared_ptr<Sampler> Sampler::Create(
		std::string key, 
		D3D11_FILTER filter, 
		D3D11_TEXTURE_ADDRESS_MODE mode, 
		unsigned int slot
	)
	{
		std::shared_ptr<Sampler> instance = mMap[key];
		if (instance == nullptr)
		{
			instance = std::make_shared<Sampler>(key, filter, mode, slot);
			mMap[key] = instance;
		}
		return instance;
	}

	Dx::Attachables::Sampler::Sampler(
		std::string key, 
		D3D11_FILTER filter, 
		D3D11_TEXTURE_ADDRESS_MODE mode,
		unsigned int slot
	) : mSlot(slot), mKey(key)
	{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = filter;
		samplerDesc.AddressU = mode;
		samplerDesc.AddressV = mode;
		samplerDesc.AddressW = mode;
		samplerDesc.MipLODBias = 0.0f;
		samplerDesc.MinLOD = 0;
		samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
		samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		samplerDesc.BorderColor[0] = 0.0f;
		samplerDesc.BorderColor[1] = 0.0f;
		samplerDesc.BorderColor[2] = 0.0f;
		samplerDesc.BorderColor[3] = 0.0f;

		Graphics::Device->CreateSamplerState(&samplerDesc, mSampler.put());
	}

	void Sampler::Attach()
	{
		if (mKey != mCurrentSampler)
		{
			mCurrentSampler = mKey;
			ID3D11SamplerState* samplers[1] = { mSampler.get() };
			Graphics::Context->PSSetSamplers(mSlot, 1, samplers);
		}
	}
}