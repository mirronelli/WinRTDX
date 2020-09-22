#pragma once
#include "pch.h"
#include "Graphics.h"
#include "ResourceManager.h"

namespace Dx::Attachables
{
	class Attachable
	{
	public:

		Attachable(uint16_t key, std::shared_ptr<Dx::Graphics> graphics) :
			m_graphics(graphics), 
			m_device(graphics->Device()),
			m_context(graphics->Context()),
			m_key(key)
		{};
		void Attach(bool force) { AttachPrivate(force); };

	protected:
		virtual void AttachPrivate(bool force) = 0;

		std::shared_ptr<Dx::Graphics>					m_graphics;
		com_ptr<ID3D11Device3>							m_device;
		com_ptr<ID3D11DeviceContext4>					m_context;
		uint16_t												m_key;
	};
}