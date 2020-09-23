#pragma once
#include "pch.h"

namespace Dx
{
	class KeyboardInput
	{
	public:
		KeyboardInput(CoreWindow window) : m_window(window)
		{
			m_window.KeyUp({ this, &KeyboardInput::KeyUp });
			m_window.KeyDown({ this, &KeyboardInput::KeyDown });
		}

		bool IsSet(Windows::System::VirtualKey key, bool autoReset = false)
		{
			bool result = m_keyStates[key];
			if (result && autoReset)
				m_keyStates[key] = false;

			return result;
		}

		void Reset(Windows::System::VirtualKey key)
		{
			m_keyStates[key] = false;
		}

	private:
		void KeyUp(CoreWindow window, KeyEventArgs args)
		{
			m_keyStates[args.VirtualKey()] = false;
		}

		void KeyDown(CoreWindow window, KeyEventArgs args)
		{
			m_keyStates[args.VirtualKey()] = true;
		}

		CoreWindow												m_window;
		std::map<Windows::System::VirtualKey, bool>	m_keyStates;
	};
}