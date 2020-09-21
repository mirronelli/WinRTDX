#pragma once
#include <pch.h>
#include <winrt/Windows.System.h>

using namespace winrt::Windows::System;

namespace Dx
{
	class KeyMap
	{
	public:

		UINT State()
		{
			return m_state;
		}

		bool IsSet(VirtualKey key)
		{
			return (m_state & static_cast<UINT>(key)) != 0;
		}

		void Reset()
		{
			m_state = 0;
		}

		void Set(VirtualKey key)
		{
			m_state |= static_cast<UINT>(key);
		}

	private:
		UINT m_state = 0;
	};

}
