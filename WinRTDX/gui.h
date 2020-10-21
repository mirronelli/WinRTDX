#pragma once
#include <pch.h>

namespace Dx
{

	class Gui
	{
	public:
		static void Init(std::shared_ptr<Dx::Graphics> graphics);
		static void ButtonDown(CoreWindow sender, PointerEventArgs e);
		static void ButtonUp(CoreWindow sender, PointerEventArgs e);
		static void KeyDown(CoreWindow sender, KeyEventArgs e);
		static void KeyUp(CoreWindow sender, KeyEventArgs e);
		static void CharacterReceived(CoreWindow sender, CharacterReceivedEventArgs e);

	private:
		static inline CoreWindow mWindow {nullptr};
		static inline HWND mHwnd;
	};

}
