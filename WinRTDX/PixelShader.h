#pragma once
#include "pch.h"
#include "Attachable.h"
#include "IO.h"
#include "CacheWithPreload.h"
#include "Structures.h"

namespace Dx::Attachables
{
	class PixelShader : public Attachable
	{
	public:
		PixelShader(Dx::Drawables::VertexType type, std::wstring filename)
			: Attachable(1), mType(type)
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
			if (force || !CacheWithPreload<PixelShader>::IsCurrent(mType))
			{
				Graphics::Context->PSSetShader(m_compiledShader.get(), nullptr, 0);
				CacheWithPreload<PixelShader>::SetCurrent(mType);
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11PixelShader>		m_compiledShader;
		Dx::Drawables::VertexType		mType;
	};
}