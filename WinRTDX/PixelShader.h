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
			m_rawDataBuffer = IO::ReadFile(filename);

			com_ptr<ID3D11PixelShader> shader;
			Graphics::Device->CreatePixelShader(
				m_rawDataBuffer.data(),
				m_rawDataBuffer.Length(),
				nullptr,
				m_compiledShader.put()
			);
		}

		uint8_t* Data()
		{
			return m_rawDataBuffer.data();
		}

		uint32_t Length()
		{
			return m_rawDataBuffer.Length();
		}

		void AttachPrivate(bool force) {
			if (force || mKey != mCurrentPixelShader)
			{
				mCurrentPixelShader = mKey;
				Graphics::Context->PSSetShader(m_compiledShader.get(), nullptr, 0);
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11PixelShader>		m_compiledShader;

		Dx::Drawables::VertexType mKey;
		inline static std::map < Dx::Drawables::VertexType, std::shared_ptr<PixelShader>> mMap;
	};
}