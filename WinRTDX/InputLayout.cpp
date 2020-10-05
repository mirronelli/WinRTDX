#include "pch.h"
#include "InputLayout.h"
#include "Structures.h"
#include "VertexShader.h"

using Dx::Drawables::VertexType;

namespace Dx::Attachables
{
	 std::shared_ptr<InputLayout> InputLayout::Get(Dx::Drawables::VertexType key)
	{
		std::shared_ptr<InputLayout> instance = mMap[key];
		if (!instance)
		{
			instance = std::make_shared<InputLayout>(key);
			mMap[key] = instance;
		}

		return instance;
	}


	InputLayout::InputLayout(VertexType key) : mKey(key)
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC>* ieds = nullptr;
		std::shared_ptr<VertexShader> vertexShader = VertexShader::Get(key);
		switch (key)
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
		if (force || mKey != mCurrentInputLayout)
		{
			mCurrentInputLayout = mKey;
			Graphics::Context->IASetInputLayout(m_inputLayout.get());
		}
	}
}