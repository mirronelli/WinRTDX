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
		static void InputLayout::Reset();
		static std::shared_ptr<InputLayout> Get(Dx::Drawables::VertexType key);
		InputLayout(Dx::Drawables::VertexType key);
		void Attach();

	private:
		com_ptr<ID3D11InputLayout>	mInputLayout;

		Dx::Drawables::VertexType mKey;
		inline static std::map < Dx::Drawables::VertexType, std::shared_ptr<InputLayout>> mMap;
	};
}
