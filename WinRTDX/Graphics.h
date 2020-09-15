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

		void StartFrame();
		void SetColor(float color[4]);
		void Present();

		com_ptr<ID3D11Device3> Device();
		com_ptr<ID3D11DeviceContext4> Context();
		CoreWindow Window();

		void SetVertexShader(com_ptr<ID3D11VertexShader> const& shader);
		void SetPixelShader(com_ptr<ID3D11PixelShader> const& shader);

		com_ptr<ID3D11VertexShader> CreateVertexShader(IBuffer buffer);
		com_ptr<ID3D11PixelShader> CreatePixelShader(IBuffer buffer);

		float Width();
		float Height();
		float MouseX();
		float MouseY();

	private:
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();
		void OnMouseMove(CoreWindow sender, PointerEventArgs args);


		// Direct3D objects.
		winrt::com_ptr<ID3D11Device3>				m_device;
		winrt::com_ptr<ID3D11DeviceContext4>	m_context;
		winrt::com_ptr<IDXGIFactory7>				m_factory;
		winrt::com_ptr<IDXGISwapChain4>			m_swapChain;
		winrt::com_ptr<ID3D11RenderTargetView>	m_renderTargetView;

		// Windows objects
		CoreWindow										m_window{ nullptr };
		float												m_width;
		float												m_height;
		float												m_dpi = 1;
		float												m_mouseX;
		float												m_mouseY;
	};
}