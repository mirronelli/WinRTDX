#pragma once
#include <pch.h>
#include "Graphics.h"

namespace Dx::Attachables
{
	class Attachable
	{
	public:
		Attachable(std::shared_ptr<Dx::Graphics>	 graphics) :
			m_graphics(graphics), 
			m_device(graphics->Device()),
			m_context(graphics->Context())
		{};
		void Attach() { AttachPrivate(); };

	protected:
		virtual void AttachPrivate() = 0;
		std::shared_ptr<Dx::Graphics>					m_graphics;
		com_ptr<ID3D11Device3>							m_device;
		com_ptr<ID3D11DeviceContext4>					m_context;
	};
}