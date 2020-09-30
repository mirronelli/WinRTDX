#pragma once
#include "pch.h"
#include "Graphics.h"
#include "ResourceManager.h"

namespace Dx::Attachables
{
	class Attachable
	{
	public:

		Attachable(int key) : m_key(key){};
		void Attach(bool force = false) { AttachPrivate(force); };

	protected:
		virtual void AttachPrivate(bool force) = 0;
		int m_key;
	};
}