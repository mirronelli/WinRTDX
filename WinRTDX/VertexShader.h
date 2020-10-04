#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include <map>
#include "IO.h"
#include "ResourceManager.h"

namespace Dx::Attachables
{
	class VertexShader : public Attachable
	{
	public:
		static std::shared_ptr<VertexShader> Load(int key, bool overwrite, std::wstring fileName)
		{
			std::shared_ptr<VertexShader> instance = std::static_pointer_cast<VertexShader>(ResourceManager::VertexShaders[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<VertexShader>(key, fileName);
				ResourceManager::VertexShaders[key] = instance;
			}

			return instance;
		}

		VertexShader(int key, std::wstring filename)
			: Attachable(key)
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
			if (force || ResourceManager::CurrentVertexShader != m_key)
			{
				Graphics::Context->VSSetShader(m_compiledShader.get(), nullptr, 0);
				ResourceManager::CurrentVertexShader = m_key;
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11VertexShader>	m_compiledShader;
	};
}