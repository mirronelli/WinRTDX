#include "pch.h"
#include "InputLayout.h"
#include "Structures.h"
#include "VertexShader.h"

using Dx::Drawables::VertexType;

namespace Dx::Attachables
{
	void InputLayout::Reset() { mMap.clear(); }
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
		case VertexType::Simple:
			ieds = &Dx::Drawables::IedsSimple;
			break;

		case VertexType::SimpleWithNormal:
			ieds = &Dx::Drawables::IedsSimpleWithNormal;
			break;

		case VertexType::Colored:
			ieds = &Dx::Drawables::IedsColored;
			break;

		case VertexType::ColoredWithNormal:
			ieds = &Dx::Drawables::IedsColoredWithNormal;
			break;

		case VertexType::Textured:
			ieds = &Dx::Drawables::IedsTextured;
			break;

		case VertexType::TexturedWithNormal:
			ieds = &Dx::Drawables::IedsTexturedWithNormal;
			break;

		case VertexType::TexturedWithNormalTangent:
			ieds = &Dx::Drawables::IedsTexturedWithNormalTangent;
			break;

		default:
			assert(false); //IED for VertexType not defined
			ieds = &Dx::Drawables::IedsSimple;
			break;
		}

		assert(vertexShader != nullptr); // make sure vertexshader for this kind is loaded

		Graphics::Device->CreateInputLayout(
			ieds->data(),
			static_cast<UINT>(ieds->size()),
			vertexShader->Data(),
			vertexShader->Length(),
			mInputLayout.put()
		);
	}

	void InputLayout::Attach()
	{
		if (mKey != mCurrentInputLayout)
		{
			mCurrentInputLayout = mKey;
			Graphics::Context->IASetInputLayout(mInputLayout.get());
		}
	}
}