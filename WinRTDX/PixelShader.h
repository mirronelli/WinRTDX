#pragma once
#include "pch.h"
#include "Attachable.h"
#include "IO.h"
#include "Structures.h"

namespace Dx::Attachables
{
	class PixelShader : public Attachable
	{
	public:
		static void Reset() { mMap.clear(); }
		static std::shared_ptr<PixelShader> Preload(Dx::Drawables::VertexType key, std::wstring fileName)
		{
			mMap[key] = std::make_shared<PixelShader>(key, fileName);
			return mMap[key];
		}

		static std::shared_ptr<PixelShader> Get(Dx::Drawables::VertexType key)
		{
			return mMap[key];
		}

		PixelShader(Dx::Drawables::VertexType key, std::wstring filename)
			: mKey(key)
		{
			mRawDataBuffer = IO::ReadFile(filename);

			com_ptr<ID3D11PixelShader> shader;
			Graphics::Device->CreatePixelShader(
				mRawDataBuffer.data(),
				mRawDataBuffer.Length(),
				nullptr,
				mCompiledShader.put()
			);
		}

		uint8_t* Data()
		{
			return mRawDataBuffer.data();
		}

		uint32_t Length()
		{
			return mRawDataBuffer.Length();
		}

		void Attach() {
			if (mKey != mCurrentPixelShader)
			{
				mCurrentPixelShader = mKey;
				Graphics::Context->PSSetShader(mCompiledShader.get(), nullptr, 0);
			}
		}

	private:
		IBuffer								mRawDataBuffer;
		com_ptr<ID3D11PixelShader>		mCompiledShader;

		Dx::Drawables::VertexType mKey;
		inline static std::map < Dx::Drawables::VertexType, std::shared_ptr<PixelShader>> mMap;
	};
}