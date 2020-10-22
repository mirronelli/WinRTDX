#pragma once
#include "pch.h"
#include "imgui.h"

namespace Dx
{
	class KeyboardInput
	{
	public:
		KeyboardInput(CoreWindow window) : mWindow(window)
		{
			mWindow.KeyUp({ this, &KeyboardInput::KeyUp });
			mWindow.KeyDown({ this, &KeyboardInput::KeyDown });
		}

		bool IsSet(Windows::System::VirtualKey key, bool autoReset = false)
		{
			bool result = mKeyStates[key];
			if (result && autoReset)
				mKeyStates[key] = false;

			return result;
		}

		void Reset(Windows::System::VirtualKey key)
		{
			mKeyStates[key] = false;
		}

	private:
		void KeyUp(CoreWindow window, KeyEventArgs args)
		{
			if (!mImguiIo.WantCaptureKeyboard)
				mKeyStates[args.VirtualKey()] = false;
		}

		void KeyDown(CoreWindow window, KeyEventArgs args)
		{
			if (!mImguiIo.WantCaptureKeyboard)
				mKeyStates[args.VirtualKey()] = true;
		}

		CoreWindow												mWindow;
		ImGuiIO& mImguiIo = ImGui::GetIO();
		std::map<Windows::System::VirtualKey, bool>	mKeyStates;
	};
}