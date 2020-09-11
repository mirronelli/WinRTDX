#pragma once
#include <winrt\Windows.UI.Core.h>

namespace Dx
{
	class DeviceResources
	{

	public:
		DeviceResources();
		void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window);
		void Resize();
		void SetDpi(float dpi);

	private:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();


		// Direct3D objects.
		winrt::com_ptr<ID3D11Device3>				m_d3dDevice;
		winrt::com_ptr<ID3D11DeviceContext4>	m_d3dContext;
		winrt::com_ptr<IDXGISwapChain4>			m_swapChain;

		// Windows objects
		winrt::Windows::UI::Core::CoreWindow	m_parentWindow{ nullptr };
		winrt::Windows::Foundation::Size			m_size;
		float												m_dpi;
	};
}