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
		static std::shared_ptr<InputLayout> Get(Dx::Drawables::VertexType key);
		InputLayout(Dx::Drawables::VertexType key);
		void AttachPrivate(bool force);

	private:
		com_ptr<ID3D11InputLayout>	m_inputLayout;

		Dx::Drawables::VertexType mKey;
		inline static std::map < Dx::Drawables::VertexType, std::shared_ptr<InputLayout>> mMap;
	};
}
