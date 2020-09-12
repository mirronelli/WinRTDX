#include "pch.h"
#include "DeviceResources.h"
#include <DxTools.h>


Dx::DeviceResources::DeviceResources()
{
	CreateDeviceResources();
};

void Dx::DeviceResources::CreateDeviceResources()
{
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	if (Dx::Tools::SdkLayersAvailable())
	{
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	D3D_FEATURE_LEVEL requestedFeatureLevels[] =
	{
		 D3D_FEATURE_LEVEL_11_1,
		 D3D_FEATURE_LEVEL_11_0
	};
	D3D_FEATURE_LEVEL detectedFeatureLevel;

	winrt::com_ptr<ID3D11Device> device;
	winrt::com_ptr<ID3D11DeviceContext> context;
	winrt::com_ptr<IDXGIAdapter4> adapter;
	winrt::com_ptr<IDXGIFactory7> dxgiFactory;


	CreateDXGIFactory1(__uuidof(IDXGIFactory7), dxgiFactory.put_void());
	adapter = Dx::Tools::GetPreferredAdapter(dxgiFactory);

	D3D11CreateDevice(
	   adapter.get(),									// Specify nullptr to use the default adapter.
	   D3D_DRIVER_TYPE_UNKNOWN,					// Create a device using the hardware graphics driver.
	   0,													// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
	   creationFlags,									// Set debug and Direct2D compatibility flags.
		requestedFeatureLevels,						// List of feature levels this app can support.
	   ARRAYSIZE(requestedFeatureLevels),		// Size of the list above.
	   D3D11_SDK_VERSION,							// Always set this to D3D11_SDK_VERSION for Windows Runtime apps.
	   device.put(),									// Returns the Direct3D device created.
	   &detectedFeatureLevel,						// Returns feature level of device created.
	   context.put()									// Returns the device immediate context.
	);

	Dx::Tools::DisplayAdapterDetails(adapter);

	m_device = device.as<ID3D11Device3>();
	m_context = context.as<ID3D11DeviceContext4>();
	m_factory = dxgiFactory.as<IDXGIFactory7>();
}

void Dx::DeviceResources::CreateWindowSizeDependentResources()
{
	if (m_swapChain != nullptr)
	{
		m_renderTargetView = nullptr;
		m_swapChain->ResizeBuffers(
			2,
			0,
			0,
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0
		);
	}
	else {
		winrt::com_ptr<IDXGISwapChain1> swapChain;
	
		DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor;
		swapChainDescriptor.Height = 0;
		swapChainDescriptor.Width = 0;
		swapChainDescriptor.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		swapChainDescriptor.Stereo = false;
		swapChainDescriptor.SampleDesc.Count = 1;
		swapChainDescriptor.SampleDesc.Quality = 0;
		swapChainDescriptor.BufferCount = 2;
		swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		swapChainDescriptor.Flags = 0;
		swapChainDescriptor.Scaling = DXGI_SCALING_NONE;
		swapChainDescriptor.AlphaMode = DXGI_ALPHA_MODE_IGNORE;

		if (
				FAILED(
					m_factory->CreateSwapChainForCoreWindow(
						m_device.get(),
						winrt::get_unknown(m_parentWindow.get()),
						&swapChainDescriptor,
						nullptr,
						swapChain.put()
					)
				)
			)
		{
			std::wostringstream debug;
			debug << "Cele zle. \n";
			OutputDebugStringW(debug.str().c_str());
			return;
		}

		m_swapChain = swapChain.as<IDXGISwapChain4>();
	}

	winrt::com_ptr<ID3D11Texture2D> backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), backBuffer.put_void());

	winrt::com_ptr<ID3D11RenderTargetView> renderTargetView;
	m_device->CreateRenderTargetView(backBuffer.get(), nullptr, renderTargetView.put());
	m_renderTargetView = renderTargetView;

	D3D11_TEXTURE2D_DESC backBufferDesc = { 0 };
	backBuffer->GetDesc(&backBufferDesc);

	D3D11_VIEWPORT viewport;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = static_cast<float>(backBufferDesc.Width);
	viewport.Height = static_cast<float>(backBufferDesc.Height);
	viewport.MinDepth = D3D11_MIN_DEPTH;
	viewport.MaxDepth = D3D11_MAX_DEPTH;

	m_context->RSSetViewports(1, &viewport);
}

void Dx::DeviceResources::SetWindow(winrt::Windows::UI::Core::CoreWindow const& window)
{
	m_parentWindow = window;
	Resize();
}

void Dx::DeviceResources::Resize()
{
	m_width = (UINT)m_parentWindow.get().Bounds().Width;
	m_height = (UINT)m_parentWindow.get().Bounds().Height;
	this->CreateWindowSizeDependentResources();
}

void Dx::DeviceResources::StartFrame()
{
	ID3D11RenderTargetView* views[] = { m_renderTargetView.get() };
	m_context->OMSetRenderTargets(
		1,
		views,
		nullptr
	);
}

void Dx::DeviceResources::SetColor(DXGI_RGBA& color)
{
	const float clearColor[4] = { color.r, color.g, color.b, color.a };
	m_context->ClearRenderTargetView(
		m_renderTargetView.get(),
		clearColor
	);
}

void Dx::DeviceResources::Present()
{
	m_swapChain->Present(1, 0);
}
