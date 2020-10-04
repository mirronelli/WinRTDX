#pragma once
#include "pch.h"
#include <map>

#include "Attachable.h"
#include "Structures.h"

namespace Dx::Attachables
{
	class InputLayout : public Dx::Attachables::Attachable
	{
	public:
		InputLayout(Dx::Drawables::VertexType type);
		void AttachPrivate(bool force);

	private:
		com_ptr<ID3D11InputLayout>	m_inputLayout;
		Dx::Drawables::VertexType mType;
	};
}
