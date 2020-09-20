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
		static std::shared_ptr<InputLayout> Create(uint16_t key, bool overwrite, std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, std::shared_ptr<VertexShader> vertexShader)
		{
			InputLayout* instancePtr = (InputLayout*)Dx::ResourceManager::GetResource(TypeIndex, key);
			std::shared_ptr<InputLayout> instance = std::shared_ptr<InputLayout>(instancePtr);

			if (overwrite || instance == nullptr)
			{
				instance = std::make_shared<InputLayout>(key, graphics, ieds, vertexShader);
				Dx::ResourceManager::SetResource(TypeIndex, key, instance.get());
			}

			return instance;
		}

		InputLayout(uint16_t key, std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, std::shared_ptr<VertexShader> vertexShader)
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
			if (force || Dx::ResourceManager::CurrentInputLayout != m_key)
			{
				m_context->IASetInputLayout(m_inputLayout.get());
				Dx::ResourceManager::CurrentInputLayout =m_key;
			}
		}

	private:
		com_ptr<ID3D11InputLayout>	m_inputLayout;

		inline static std::type_index TypeIndex = std::type_index(typeid(std::string)); // dummy value, is overwritten at runtime
	};
}
