#include "pch.h"
#include "InputLayout.h"
#include "Structures.h"
#include "VertexShader.h"
#include "Cache.h"

namespace Dx::Attachables
{
	InputLayout::InputLayout(
		Dx::Drawables::VertexType type)
		: Attachable(1), mType(type)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC>* ieds = nullptr;
		std::shared_ptr<VertexShader> vertexShader;
		switch (type)
		{
			case Dx::Drawables::VertexType::ColoredWithNormal:
				ieds = &Dx::Drawables::IedsColoredWithNormal;
				vertexShader = std::static_pointer_cast<VertexShader>(ResourceManager::VertexShaders[1]);
				break;

			case Dx::Drawables::VertexType::Colored:
				ieds = &Dx::Drawables::IedsColored;
				vertexShader = std::static_pointer_cast<VertexShader>(ResourceManager::VertexShaders[2]);
				break;

			default:
				ieds = &Dx::Drawables::IedsSimple;
				vertexShader = vertexShader = std::static_pointer_cast<VertexShader>(ResourceManager::VertexShaders[2]);
				break;
		}

		Graphics::Device->CreateInputLayout(
			ieds->data(),
			static_cast<UINT>(ieds->size()),
			vertexShader->Data(),
			vertexShader->Length(),
			m_inputLayout.put()
		);
	}

	void InputLayout::AttachPrivate(bool force)
	{
		if (force || !Cache<InputLayout>::IsCurrent(mType))
		{
			Graphics::Context->IASetInputLayout(m_inputLayout.get());
			Cache<InputLayout>::SetCurrent(mType);
		}
	}
}