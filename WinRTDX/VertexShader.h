#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include <map>
#include "IO.h"

namespace Dx::Attachables
{
	class VertexShader : public Attachable
	{
	public:
		static std::shared_ptr<VertexShader> Load(uint16_t key, bool overwrite, std::shared_ptr<Graphics> graphics, std::wstring fileName)
		{
			std::shared_ptr<VertexShader> instance = std::static_pointer_cast<VertexShader>(ResourceManager::VertexShaders[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<VertexShader>(key, graphics, fileName);
				ResourceManager::VertexShaders[key] = instance;
			}

			return instance;
		}

		VertexShader(uint16_t key, std::shared_ptr<Graphics> graphics, std::wstring filename)
			: Attachable(key, graphics)
		{
			m_rawDataBuffer = IO::ReadFile(filename);

			com_ptr<ID3D11VertexShader> shader;
			m_device->CreateVertexShader(
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
			if (force || ResourceManager::CurrentVertexShader != m_key)
			{
				m_context->VSSetShader(m_compiledShader.get(), nullptr, 0);
				ResourceManager::CurrentVertexShader = m_key;
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11VertexShader>	m_compiledShader;
	};
}