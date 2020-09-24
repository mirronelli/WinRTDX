#pragma once
#include <winrt\Windows.UI.Core.h>

using namespace winrt;
using namespace winrt::Windows::UI::Core;
using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Foundation;

namespace Dx
{
	class Graphics
	{

	public:
		Graphics();
		void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window);
		void Resize();

		void StartFrame(float color[4]);
		void Present();

		com_ptr<ID3D11Device3> Device();
		com_ptr<ID3D11DeviceContext4> Context();
		CoreWindow Window();

		float Width();
		float Height();
		float MouseX();
		float MouseY();

	private:
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void OnMouseMove(CoreWindow sender, PointerEventArgs args);


		// Direct3D objects.
		com_ptr<ID3D11Device3>						m_device;
		com_ptr<ID3D11DeviceContext4>				m_context;
		com_ptr<IDXGIFactory3>						m_factory;
		com_ptr<IDXGISwapChain4>					m_swapChain;
		com_ptr<ID3D11RenderTargetView>			m_renderTargetView;
		com_ptr<ID3D11DepthStencilView>			m_depthStencilView;

		// Windows objects
		CoreWindow										m_window{ nullptr };
		float												m_width;
		float												m_height;
		float												m_dpi = 1;
		float												m_mouseX{ 0 };
		float												m_mouseY{ 0 };
	};
}