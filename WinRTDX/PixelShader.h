#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include "IO.h"
#include "ResourceManager.h"

namespace Dx::Attachables
{
	class PixelShader : public Attachable
	{
	public:
		static std::shared_ptr<PixelShader> Load(int key, bool overwrite, std::wstring filename)
		{
			std::shared_ptr<PixelShader> instance = std::static_pointer_cast<PixelShader>(ResourceManager::PixelShaders[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<PixelShader>(key, filename);
				ResourceManager::PixelShaders[key] = instance;
			}

			return instance;
		}

		PixelShader(int key, std::wstring filename)
			: Attachable(key)
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
			if (force || ResourceManager::CurrentPixelShader != m_key)
			{
				Graphics::Context->PSSetShader(m_compiledShader.get(), nullptr, 0);
				ResourceManager::CurrentPixelShader = m_key;
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11PixelShader>		m_compiledShader;
	};
}