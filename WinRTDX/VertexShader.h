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
			m_rawDataBuffer = IO::ReadFile(filename);

			com_ptr<ID3D11VertexShader> shader;
			Graphics::Device->CreateVertexShader(
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

		void AttachPrivate(bool force)
		{
			if (force || mKey != mCurrentVertexShader)
			{
				mCurrentVertexShader = mKey;
				Graphics::Context->VSSetShader(m_compiledShader.get(), nullptr, 0);
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11VertexShader>	m_compiledShader;

		Dx::Drawables::VertexType mKey;
		inline static std::map < Dx::Drawables::VertexType, std::shared_ptr<VertexShader>> mMap;
	};
}