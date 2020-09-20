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
		static std::shared_ptr<VertexShader> Load(uint16_t key, bool overwrite, std::shared_ptr<Graphics> graphics, std::wstring fileName)
		{
			VertexShader* instancePtr = (VertexShader*)Dx::ResourceManager::GetResource(TypeIndex, key);
			std::shared_ptr<VertexShader> instance = std::shared_ptr<VertexShader>(instancePtr);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<VertexShader>(key, graphics, fileName);
				Dx::ResourceManager::SetResource(TypeIndex, key, instance.get());
			}

			return instance;
		}

		VertexShader(uint16_t key, std::shared_ptr<Graphics> graphics, std::wstring filename)
			: Attachable(key, graphics)
		{
			VertexShader::TypeIndex = std::type_index(typeid(VertexShader));
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
			if (force || Dx::ResourceManager::GetCurrentInstance(TypeIndex) != m_key)
			{
				m_context->VSSetShader(m_compiledShader.get(), nullptr, 0);
				Dx::ResourceManager::SetCurrentInstance(TypeIndex, m_key);
			}
		}

	private:
		IBuffer								m_rawDataBuffer;
		com_ptr<ID3D11VertexShader>	m_compiledShader;
		std::shared_ptr<Dx::Graphics>	m_graphics;

		inline static std::type_index TypeIndex = std::type_index(typeid(std::string)); // dummy value, is overwritten at runtime
	};
}