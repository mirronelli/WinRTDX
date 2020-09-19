#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include "VertexShader.h"
#include <map>

namespace Dx::Attachables
{
	class InputLayout : public Attachable
	{
	public:
		static void ClearCache() { m_instances.clear(); m_current_instance_key.clear(); }
		static std::shared_ptr<InputLayout> Create(std::wstring const& key, bool overwrite, std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, std::shared_ptr<VertexShader> vertexShader)
		{
			std::shared_ptr<InputLayout> instance = m_instances[key];

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<InputLayout>(key, graphics, ieds, vertexShader);
				m_instances[key] = instance;
			}

			return instance;
		}

		InputLayout(std::wstring const& key, std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, std::shared_ptr<VertexShader> vertexShader)
			: Attachable(key, graphics)
		{
			m_graphics->Device()->CreateInputLayout(
				ieds.data(),
				static_cast<UINT>(ieds.size()),
				vertexShader->Data(),
				vertexShader->Length(),
				m_inputLayout.put()
			);
		}

		void AttachPrivate(bool force)
		{
			if (force || m_current_instance_key != m_key)
			{
				m_context->IASetInputLayout(m_inputLayout.get());
				m_current_instance_key = m_key;
			}
		}

	private:
		inline static std::map<std::wstring, std::shared_ptr<InputLayout>> m_instances = {};
		inline static std::wstring m_current_instance_key = {};
		com_ptr<ID3D11InputLayout>					m_inputLayout;
	};
}
