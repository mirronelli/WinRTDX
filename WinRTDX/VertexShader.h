#pragma once
#include "pch.h"
#include "Attachable.h"
#include "IO.h"
#include "CacheWithPreload.h"
#include "Structures.h"

namespace Dx::Attachables
{
	class VertexShader : public Attachable, public CacheWithPreload<VertexShader>
	{
	public:
		VertexShader(Dx::Drawables::VertexType type, std::wstring filename)
			: CacheWithPreload<VertexShader>(type)
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
			if (force || !VertexShader::IsCurrent(mKey))
			{
				Graphics::Context->VSSetShader(m_compiledShader.get(), nullptr, 0);
				VertexShader::SetCurrent(mKey);
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11VertexShader>	m_compiledShader;
	};
}