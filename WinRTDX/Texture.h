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
		static std::shared_ptr<Texture> Preload(std::string key, hstring filename, unsigned int slot = 0)
		{
			std::shared_ptr<Texture> instance = std::make_shared<Texture>(key, filename, slot);
			mMap[key] = instance;
			return instance;
		}

		static std::shared_ptr<Texture> Get(std::string key)
		{
			return mMap[key];
		}

		Texture(std::string key, hstring filename, UINT slot)
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
		}

		void Attach()
		{
			Attach(Dx::Drawables::TextureType::Diffuse);
		}

		void Attach(Dx::Drawables::TextureType type)
		{

			if (mKey != mCurrentTextures[type])
			{
				mCurrentTextures[type] = mKey;
				ID3D11ShaderResourceView* textureViews[1] = { mTextureView.get() };
				Graphics::Context->PSSetShaderResources(mSlot, 1, textureViews);
			}
		}

	private:
		IBuffer											mRawDataBuffer;
		com_ptr<ID3D11Texture2D>					mTexture;
		com_ptr<ID3D11Resource>						mTextureWic;
		com_ptr<ID3D11ShaderResourceView>		mTextureView;
		UINT												mSlot;

		std::string mKey;
		inline static std::map < std::string, std::shared_ptr<Texture>> mMap;
	};
}