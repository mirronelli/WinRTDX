#pragma once
#include <pch.h>
#include "Graphics.h"
#include "Attachable.h"
#include "IO.h"

namespace Dx::Attachables
{
	class PixelShader : public Attachable
	{
	public:
		static std::shared_ptr<PixelShader> Load(uint16_t key, bool overwrite, std::shared_ptr<Graphics> graphics, std::wstring filename)
		{
			std::shared_ptr<PixelShader> instance = std::static_pointer_cast<PixelShader>(ResourceManager::PixelShaders[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<PixelShader>(key, graphics, filename);
				ResourceManager::PixelShaders[key] = instance;
			}

			return instance;
		}

		PixelShader(uint16_t key, std::shared_ptr<Graphics> graphics, std::wstring filename)
			: Attachable(key, graphics)
		{
			m_rawDataBuffer = IO::ReadFile(filename);

			com_ptr<ID3D11PixelShader> shader;
			m_device->CreatePixelShader(
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
			if (force || ResourceManager::CurrentPixelShader != m_key)
			{
				m_context->PSSetShader(m_compiledShader.get(), nullptr, 0);
				ResourceManager::CurrentPixelShader = m_key;
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11PixelShader>		m_compiledShader;
		std::shared_ptr<Dx::Graphics>	m_graphics;
	};
}