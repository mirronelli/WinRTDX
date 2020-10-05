#pragma once
#include "pch.h"
#include "Attachable.h"
#include <map>
#include "IO.h"
#include "dds.h"
namespace Dx::Attachables
{
	class Texture : public Attachable
	{
	public:
		static std::shared_ptr<Texture> Preload(std::string key, std::wstring filename)
		{
			std::shared_ptr<Texture> instance = std::make_shared<Texture>(key, filename);
			mMap[key] = instance;
			return instance;
		}

		static std::shared_ptr<Texture> Get(std::string key)
		{
			return mMap[key];
		}

		Texture(std::string key, std::wstring filename, UINT slot = 0)
			: 
			m_slot(slot)
		{
			m_rawDataBuffer = IO::ReadFile(filename);
			DirectX::DDS_IMAGE* textureStruct = (DirectX::DDS_IMAGE*)m_rawDataBuffer.data();


			// Texture
			D3D11_TEXTURE2D_DESC desc = {};
			desc.Width = textureStruct->header.width;
			desc.Height = textureStruct->header.height;
			desc.MipLevels = desc.ArraySize = textureStruct->header.mipMapCount;
			desc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;

			D3D11_SUBRESOURCE_DATA srd = {};
			srd.pSysMem = &textureStruct->data;
			srd.SysMemPitch = textureStruct->header.pitchOrLinearSize;
		
			Graphics::Device->CreateTexture2D(&desc, &srd, m_texture.put());

			// Texture view
			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
			viewDesc.Format = desc.Format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MipLevels = desc.MipLevels;
			viewDesc.Texture2D.MostDetailedMip = 0;

			Graphics::Device->CreateShaderResourceView(m_texture.get(), &viewDesc, m_textureView.put());

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

			Graphics::Device->CreateSamplerState(&samplerDesc, m_sampler.put());
		}

		void AttachPrivate(bool force)
		{
			if (force || mKey != mCurrentTexture)
			{
				mCurrentTexture = mKey;
				ID3D11ShaderResourceView* textureViews[1] = { m_textureView.get() };
				Graphics::Context->PSSetShaderResources(m_slot, 1, textureViews);

				ID3D11SamplerState* samplers[1] = { m_sampler.get() };
				Graphics::Context->PSSetSamplers(m_slot, 1, samplers);
			}
		}

	private:
		IBuffer											m_rawDataBuffer;
		com_ptr<ID3D11Texture2D>					m_texture;
		com_ptr<ID3D11ShaderResourceView>		m_textureView;
		com_ptr<ID3D11SamplerState>				m_sampler;
		UINT												m_slot;

		std::string mKey;
		inline static std::map < std::string, std::shared_ptr<Texture>> mMap;
	};
}