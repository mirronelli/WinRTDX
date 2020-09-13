#pragma once
#include <winrt\Windows.UI.Core.h>
#include "Loader.h"

using namespace winrt;
using namespace winrt::Windows::UI::Core;

namespace Dx
{
	class Graphics
	{

	public:
		Graphics();
		void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window);
		void Resize();

		void StartFrame();
		void SetColor(DXGI_RGBA& color);
		void Present();


		com_ptr<ID3D11VertexShader> LoadVertexShader(std::wstring const& filename);
		void SetVertexShader(com_ptr<ID3D11VertexShader> const& shader);

		com_ptr<ID3D11PixelShader> LoadPixelShader(std::wstring const& filename);
		void SetPixelShader(com_ptr<ID3D11PixelShader> const& shader);

	private:
		void CreateDeviceResources();
		void CreateWindowSizeDependentResources();


		// Direct3D objects.
		winrt::com_ptr<ID3D11Device3>				m_device;
		winrt::com_ptr<ID3D11DeviceContext4>	m_context;
		winrt::com_ptr<IDXGIFactory7>				m_factory;
		winrt::com_ptr<IDXGISwapChain4>			m_swapChain;
		winrt::com_ptr<ID3D11RenderTargetView>	m_renderTargetView;

		// Windows objects
		winrt::agile_ref<CoreWindow>				m_parentWindow{ nullptr };
		UINT												m_width;
		UINT												m_height;
		float												m_dpi = 1;

		// My objects
		Loader											m_loader{ nullptr };
	};
}