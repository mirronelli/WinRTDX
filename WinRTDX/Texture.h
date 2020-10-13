#pragma once
#include "pch.h"
#include "Attachable.h"
#include <map>
#include "IO.h"
#include "dds.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"
#include "DxTools.h"
#include <winerror.h>

namespace Dx::Attachables
{
	class Texture : public Attachable
	{
	public:
		static void Reset() { mMap.clear(); }
		static std::shared_ptr<Texture> Preload(std::string key, hstring filename)
		{
			std::shared_ptr<Texture> instance = std::make_shared<Texture>(key, filename);
			mMap[key] = instance;
			return instance;
		}

		static std::shared_ptr<Texture> Get(std::string key)
		{
			return mMap[key];
		}

		Texture(std::string key, hstring filename, UINT slot = 0)
			: 
			mKey(key),
			mSlot(slot)
		{
			if (Dx::Tools::ends_with(filename, L".dds"))
			{
				Dx::Tools::ThrowIfFailed(
					DirectX::CreateDDSTextureFromFile(Graphics::Device.get(), filename.c_str(), mTextureWic.put(), mTextureView.put())
				);
			}
			else
			{
				Dx::Tools::ThrowIfFailed(
					DirectX::CreateWICTextureFromFile(Graphics::Device.get(), filename.c_str(), mTextureWic.put(), mTextureView.put())
				);
			}
			mTexture = mTextureWic.as< ID3D11Texture2D>();

			//mRawDataBuffer = IO::ReadFile(filename.c_str());
			//DirectX::DDS_IMAGE* textureStruct = (DirectX::DDS_IMAGE*)mRawDataBuffer.data();


			//// Texture
			//D3D11_TEXTURE2D_DESC desc = {};
			//desc.Width = textureStruct->header.width;
			//desc.Height = textureStruct->header.height;
			//desc.MipLevels = desc.ArraySize = textureStruct->header.mipMapCount;
			//desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			//desc.SampleDesc.Count = 1;
			//desc.Usage = D3D11_USAGE_DEFAULT;
			//desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			//desc.CPUAccessFlags = 0;
			//desc.MiscFlags = 0;

			//D3D11_SUBRESOURCE_DATA srd = {};
			//srd.pSysMem = &textureStruct->data;
			//srd.SysMemPitch = textureStruct->header.pitchOrLinearSize;
		
			//Graphics::Device->CreateTexture2D(&desc, &srd, mTexture.put());

			//// Texture view
			//D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
			//viewDesc.Format = desc.Format;
			//viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			//viewDesc.Texture2D.MipLevels = desc.MipLevels;
			//viewDesc.Texture2D.MostDetailedMip = 0;

			//Graphics::Device->CreateShaderResourceView(mTexture.get(), &viewDesc, mTextureView.put());

			// Sampler
			D3D11_SAMPLER_DESC samplerDesc = {};
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
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

		void Attach()
		{
			if (mKey != mCurrentTexture)
			{
				mCurrentTexture = mKey;
				ID3D11ShaderResourceView* textureViews[1] = { mTextureView.get() };
				Graphics::Context->PSSetShaderResources(mSlot, 1, textureViews);

				ID3D11SamplerState* samplers[1] = { mSampler.get() };
				Graphics::Context->PSSetSamplers(mSlot, 1, samplers);
			}
		}

	private:
		IBuffer											mRawDataBuffer;
		com_ptr<ID3D11Texture2D>					mTexture;
		com_ptr<ID3D11Resource>						mTextureWic;
		com_ptr<ID3D11ShaderResourceView>		mTextureView;
		com_ptr<ID3D11SamplerState>				mSampler;
		UINT												mSlot;

		std::string mKey;
		inline static std::map < std::string, std::shared_ptr<Texture>> mMap;
	};
}