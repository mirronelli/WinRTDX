#pragma once
#include <pch.h>
#include "Graphics.h"
#include "Attachable.h"
#include <map>
#include "IO.h"

namespace Dx::Attachables
{
	class VertexShader : public Attachable
	{
	public:
		static void ClearCache() { m_instances.clear(); m_current_instance_key.clear(); }
		static std::shared_ptr<VertexShader> Load(std::wstring fileName, bool overwrite, std::shared_ptr<Graphics> graphics)
		{
			std::shared_ptr<VertexShader> instance = m_instances[fileName];

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<VertexShader>(graphics, fileName);
				m_instances[fileName] = instance;
			}

			return instance;
		}

		VertexShader(std::shared_ptr<Graphics> graphics, std::wstring filename)
			: Attachable(filename, graphics)
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
			if (force || m_current_instance_key != m_key)
			{
				m_context->VSSetShader(m_compiledShader.get(), nullptr, 0);
				m_current_instance_key = m_key;
			}
		}

	private:
		inline static std::map<std::wstring, std::shared_ptr<VertexShader>> m_instances = {};
		inline static std::wstring m_current_instance_key = {};

		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11VertexShader>	m_compiledShader;
		std::shared_ptr<Dx::Graphics>	m_graphics;
	};
}