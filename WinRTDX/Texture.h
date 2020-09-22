#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include <map>
#include "IO.h"
#include "dds.h"

namespace Dx::Attachables
{
	class Texture : public Attachable
	{


	public:
		static std::shared_ptr<Texture> Load(uint16_t key, bool overwrite, std::shared_ptr<Graphics> graphics, std::wstring fileName, UINT slot=0)
		{
			std::shared_ptr<Texture> instance = std::static_pointer_cast<Texture>(ResourceManager::Textures[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<Texture>(key, graphics, fileName, slot);
				ResourceManager::Textures[key] = instance;
			}

			return instance;
		}

		Texture(uint16_t key, std::shared_ptr<Graphics> graphics, std::wstring filename, UINT slot)
			: Attachable(key, graphics),
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
		
			m_device->CreateTexture2D(&desc, &srd, m_texture.put());

			// Texture view
			D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc = {};
			viewDesc.Format = desc.Format;
			viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			viewDesc.Texture2D.MipLevels = desc.MipLevels;
			viewDesc.Texture2D.MostDetailedMip = 0;

			m_device->CreateShaderResourceView(m_texture.get(), &viewDesc, m_textureView.put());

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

			m_device->CreateSamplerState(&samplerDesc, m_sampler.put());
		}

		void AttachPrivate(bool force)
		{
			if (force || ResourceManager::CurrentTexture != m_key)
			{
				ID3D11ShaderResourceView* textureViews[1] = { m_textureView.get() };
				m_context->PSSetShaderResources(m_slot, 1, textureViews);

				ID3D11SamplerState* samplers[1] = { m_sampler.get() };
				m_context->PSSetSamplers(m_slot, 1, samplers);

				ResourceManager::CurrentTexture = m_key;
			}
		}

	private:
		IBuffer											m_rawDataBuffer;
		com_ptr<ID3D11Texture2D>					m_texture;
		com_ptr<ID3D11ShaderResourceView>		m_textureView;
		com_ptr<ID3D11SamplerState>				m_sampler;
		UINT												m_slot;
	};
}