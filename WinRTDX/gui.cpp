#include "pch.h"
#include "gui.h"
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
using namespace winrt::Windows::UI::Input;

namespace Dx
{ 
	struct
		__declspec(uuid("45D64A29-A63E-4CB6-B498-5781D298CB4F"))
		__declspec(novtable)
		ICoreWindowInterop : public ::IUnknown
	{
		virtual HRESULT STDMETHODCALLTYPE get_WindowHandle(HWND* hwnd) = 0;
		virtual HRESULT STDMETHODCALLTYPE put_MessageHandled(unsigned char value) = 0;
	};

	void Gui::Init(std::shared_ptr<Dx::Graphics> graphics)
	{
		mWindow = graphics->Window();
		mWindow.PointerPressed(&ButtonDown);
		mWindow.PointerReleased(&ButtonUp);
		mWindow.KeyDown(&KeyDown);
		mWindow.KeyUp(&KeyUp);
		mWindow.CharacterReceived(&CharacterReceived);

		com_ptr<ICoreWindowInterop> interop{};
		winrt::get_unknown(graphics->Window())->QueryInterface<ICoreWindowInterop>(interop.put());

		interop->get_WindowHandle(&mHwnd);

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;

		ImGui::StyleColorsDark();
		// Setup Platform/Renderer backends
		ImGui_ImplWin32_Init(mHwnd);
		ImGui_ImplDX11_Init(graphics->Device.get(), graphics->Context.get());
	}

	void Gui::ButtonDown(CoreWindow sender, PointerEventArgs e) 
	{
		auto kind = e.CurrentPoint().Properties().PointerUpdateKind();
		UINT message = 0;

		switch (kind)
		{ 
			case PointerUpdateKind::LeftButtonPressed:
				message = WM_LBUTTONDOWN;
				break;
			case PointerUpdateKind::RightButtonPressed:
				message = WM_RBUTTONDOWN;
				break;
			case PointerUpdateKind::MiddleButtonPressed:
				message = WM_MBUTTONDOWN;
				break;
		}

		if (message > 0)
			ImGui_ImplWin32_WndProcHandler(mHwnd, message, 0, 0);
	}

	void Gui::ButtonUp(CoreWindow sender, PointerEventArgs e)
	{
		auto kind = e.CurrentPoint().Properties().PointerUpdateKind();
		UINT message = 0;

		switch (kind)
		{
		case PointerUpdateKind::LeftButtonReleased:
			message = WM_LBUTTONUP;
			break;
		case PointerUpdateKind::RightButtonReleased:
			message = WM_RBUTTONUP;
			break;
		case PointerUpdateKind::MiddleButtonReleased:
			message = WM_MBUTTONUP;
			break;
		}

		if (message > 0)
			ImGui_ImplWin32_WndProcHandler(mHwnd, message, 0, 0);
	}

	void Gui::KeyDown(CoreWindow sender, KeyEventArgs e)
	{
		ImGui_ImplWin32_WndProcHandler(mHwnd, WM_KEYDOWN, (UINT)e.VirtualKey(), 0);
	}

	void Gui::KeyUp(CoreWindow sender, KeyEventArgs e)
	{
		ImGui_ImplWin32_WndProcHandler(mHwnd, WM_KEYUP, (UINT)e.VirtualKey(), 0);
	}

	void Gui::CharacterReceived(CoreWindow sender, CharacterReceivedEventArgs e)
	{
		ImGui_ImplWin32_WndProcHandler(mHwnd, WM_CHAR, (UINT)e.KeyCode(), 0);
		e.Handled(true);
	}
}