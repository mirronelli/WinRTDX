#pragma once
#include "pch.h"
#include "Graphics.h"
#include "Attachable.h"

namespace Dx::Attachables
{
	class InputLayout : public Attachable
	{
	public:
		InputLayout(std::shared_ptr<Graphics> graphics, std::vector<D3D11_INPUT_ELEMENT_DESC> const& ieds, IBuffer vertexShaderByteCode) : Attachable(graphics)
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
		com_ptr<ID3D11InputLayout>			m_inputLayout;
	};
}
