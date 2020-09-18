#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"
#include <map>

namespace Dx::Attachables
{
	class InputLayout : public Attachable
	{
	public:
		static std::shared_ptr<InputLayout> Create(std::string const& key, bool overwrite, std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, IBuffer vertexShaderByteCode)
		{
			std::shared_ptr<InputLayout> instance = m_instances[key];

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<InputLayout>(graphics, ieds, vertexShaderByteCode);
				m_instances[key] = instance;
			}

			return instance;
		}

		InputLayout(std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, IBuffer vertexShaderByteCode)
			: Attachable(graphics)
		{
			m_graphics->Device()->CreateInputLayout(
				ieds.data(),
				static_cast<UINT>(ieds.size()),
				vertexShaderByteCode.data(),
				vertexShaderByteCode.Length(),
				m_inputLayout.put()
			);
		}

		void AttachPrivate()
		{
			m_context->IASetInputLayout(m_inputLayout.get());
		}

	private:
		inline static std::map<std::string, std::shared_ptr<InputLayout>> m_instances = {};
		com_ptr<ID3D11InputLayout>					m_inputLayout;
	};
}
