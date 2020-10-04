#include "pch.h"
#include "InputLayout.h"
#include "Structures.h"
#include "VertexShader.h"
#include "Cache.h"
#include "CacheWithPreload.h"

using Dx::Drawables::VertexType;

namespace Dx::Attachables
{
	InputLayout::InputLayout(
		VertexType type)
		: Attachable(1), Cache<VertexType, InputLayout>(type)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC>* ieds = nullptr;
		std::shared_ptr<VertexShader> vertexShader = CacheWithPreload<VertexShader>::Get(type);
		switch (type)
		{
			case VertexType::ColoredWithNormal:
				ieds = &Dx::Drawables::IedsColoredWithNormal;
				break;

			case VertexType::Colored:
				ieds = &Dx::Drawables::IedsColored;
				break;

			case VertexType::SimpleWithNormal:
				ieds = &Dx::Drawables::IedsSimpleWithNormal;
				break;

			default:
				ieds = &Dx::Drawables::IedsSimple;
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
		if (force || !InputLayout::IsCurrent(mType))
		{
			Graphics::Context->IASetInputLayout(m_inputLayout.get());
			InputLayout::SetCurrent(mType);
		}
	}
}