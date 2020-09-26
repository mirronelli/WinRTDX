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
		static std::shared_ptr<InputLayout> Create(int key, bool overwrite, std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, std::shared_ptr<VertexShader> vertexShader)
		{
			std::shared_ptr<InputLayout> instance = std::static_pointer_cast<InputLayout>(ResourceManager::InputLayouts[key]);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<InputLayout>(key, graphics, ieds, vertexShader);
				ResourceManager::InputLayouts[key] = instance;
			}

			return instance;
		}

		InputLayout(int key, std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, std::shared_ptr<VertexShader> vertexShader)
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
			if (force || ResourceManager::CurrentInputLayout != m_key)
			{
				m_context->IASetInputLayout(m_inputLayout.get());
				ResourceManager::CurrentInputLayout =m_key;
			}
		}

	private:
		com_ptr<ID3D11InputLayout>	m_inputLayout;
	};
}
