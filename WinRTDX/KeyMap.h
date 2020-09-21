#pragma once
#include <pch.h>
#include <winrt/Windows.System.h>

using namespace winrt::Windows::System;

namespace Dx
{
	class KeyMap
	{
	public:
		bool IsSet(VirtualKey key)
		{
			return m_keyStates[key];
		}

		void Reset()
		{
			m_keyStates.clear();
		}

		void Set(VirtualKey key)
		{
			m_keyStates[key] = true;
		}

	private:
		std::map<VirtualKey,bool>   m_keyStates;
	};

}
