#pragma once
#include "pch.h"
#include "Attachable.h"
#include "IO.h"
#include "Structures.h"

namespace Dx::Attachables
{
	class VertexShader : public Attachable
	{
	public:
		static void Reset() { mMap.clear(); }
		static std::shared_ptr<VertexShader> Preload(Dx::Drawables::VertexType key, std::wstring fileName)
		{
			mMap[key] = std::make_shared<VertexShader>(key, fileName);
			return mMap[key];
		}

		static std::shared_ptr<VertexShader> Get(Dx::Drawables::VertexType key)
		{
			return mMap[key];
		}

		VertexShader(Dx::Drawables::VertexType key, std::wstring filename)
			: mKey(key)
		{
			mRawDataBuffer = IO::ReadFile(filename);

			com_ptr<ID3D11VertexShader> shader;
			Graphics::Device->CreateVertexShader(
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

		void Attach()
		{
			if (mKey != mCurrentVertexShader)
			{
				mCurrentVertexShader = mKey;
				Graphics::Context->VSSetShader(mCompiledShader.get(), nullptr, 0);
			}
		}

	private:
		IBuffer								mRawDataBuffer;
		com_ptr<ID3D11VertexShader>	mCompiledShader;

		Dx::Drawables::VertexType mKey;
		inline static std::map < Dx::Drawables::VertexType, std::shared_ptr<VertexShader>> mMap;
	};
}