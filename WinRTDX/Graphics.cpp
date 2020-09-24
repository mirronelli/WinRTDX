#include "pch.h"
#include "Graphics.h"
#include "DxTools.h"

using namespace winrt::Windows::Storage::Streams;
using namespace winrt::Windows::Foundation;

Dx::Graphics::Graphics()
{
	CreateDeviceResources();
};

void Dx::Graphics::CreateDeviceResources()
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
	//winrt::com_ptr<IDXGIAdapter4> adapter;
	//winrt::com_ptr<IDXGIFactory7> dxgiFactory;


	//CreateDXGIFactory1(__uuidof(IDXGIFactory7), dxgiFactory.put_void());
	//adapter = Dx::Tools::GetPreferredAdapter(dxgiFactory);

	D3D11CreateDevice(
		nullptr,									// Specify nullptr to use the default adapter.
		D3D_DRIVER_TYPE_HARDWARE,					// Create a device using the hardware graphics driver.
		0,													// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		creationFlags,									// Set debug and Direct2D compatibility flags.
		requestedFeatureLevels,						// List of feature levels this app can support.
		ARRAYSIZE(requestedFeatureLevels),		// Size of the list above.
		D3D11_SDK_VERSION,							// Always set this to D3D11_SDK_VERSION for Windows Runtime apps.
		device.put(),									// Returns the Direct3D device created.
		&detectedFeatureLevel,						// Returns feature level of device created.
		context.put()									// Returns the device immediate context.
	);

	m_device = device.as<ID3D11Device3>();
	m_context = context.as<ID3D11DeviceContext4>();

	com_ptr<IDXGIDevice3> dxgiDevice;
	dxgiDevice = m_device.as<IDXGIDevice3>();

	com_ptr<IDXGIAdapter> dxgiAdapter;
	dxgiDevice->GetAdapter(dxgiAdapter.put());

	dxgiAdapter->GetParent(IID_PPV_ARGS(&m_factory));

	Dx::Tools::DisplayAdapterDetails(dxgiAdapter);
}

void Dx::Graphics::CreateWindowSizeDependentResources()
{
#pragma region swap_chain
	if (m_swapChain != nullptr)
	{
		m_renderTargetView = nullptr;
		m_depthStencilView = nullptr;
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

		DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor { 0 };
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
		swapChainDescriptor.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		if (
			FAILED(
				m_factory->CreateSwapChainForCoreWindow(
					m_device.get(),
					winrt::get_unknown(m_window),
					&swapChainDescriptor,
					nullptr,
					swapChain.put()
				)
			)
			)
		{
			return;
		}

		m_swapChain = swapChain.as<IDXGISwapChain4>();
	}
#pragma endregion

#pragma region render target view

	//  represents the active backbuffer
	com_ptr<ID3D11Texture2D> backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), backBuffer.put_void());

	com_ptr<ID3D11RenderTargetView> renderTargetView;
	m_device->CreateRenderTargetView(backBuffer.get(), nullptr, renderTargetView.put());
	m_renderTargetView = renderTargetView;

#pragma endregion

#pragma region view port
	//represents the part of render target view we will render to

	D3D11_VIEWPORT viewports[1];
	viewports[0].TopLeftX = 0.0f;
	viewports[0].TopLeftY = 0.0f;
	viewports[0].Width = m_width;
	viewports[0].Height = m_height;
	viewports[0].MinDepth = D3D11_MIN_DEPTH;
	viewports[0].MaxDepth = D3D11_MAX_DEPTH;

	m_context->RSSetViewports(1, viewports);

#pragma endregion

#pragma region depth buffer

	D3D11_DEPTH_STENCIL_DESC depthBufferDesc{ 0 };
	depthBufferDesc.DepthEnable = true;
	depthBufferDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS;
	depthBufferDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	com_ptr<ID3D11DepthStencilState> depthStencilState;
	m_device->CreateDepthStencilState(&depthBufferDesc, depthStencilState.put());
	m_context->OMSetDepthStencilState(depthStencilState.get(), 1);

	com_ptr<ID3D11Texture2D> depthBufferTexture;
	D3D11_TEXTURE2D_DESC depthBufferTextureDesc{ 0 };
	depthBufferTextureDesc.Width = static_cast<UINT>(m_width);
	depthBufferTextureDesc.Height = static_cast<UINT>(m_height);
	depthBufferTextureDesc.MipLevels = 1;
	depthBufferTextureDesc.ArraySize = 1;
	depthBufferTextureDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	depthBufferTextureDesc.SampleDesc.Count = 1;
	depthBufferTextureDesc.SampleDesc.Quality = 0;
	depthBufferTextureDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	depthBufferTextureDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
	m_device->CreateTexture2D(&depthBufferTextureDesc, nullptr, depthBufferTexture.put());

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	m_device->CreateDepthStencilView(depthBufferTexture.get(), &depthStencilViewDesc, m_depthStencilView.put());

#pragma endregion
}

void Dx::Graphics::OnMouseMove(CoreWindow sender, PointerEventArgs args)
{
	m_mouseX = 4 * (args.CurrentPoint().Position().X - m_width / 2) / m_width;
	m_mouseY = 4 *(- args.CurrentPoint().Position().Y + m_height / 2) / m_height;
}

void Dx::Graphics::SetWindow(winrt::Windows::UI::Core::CoreWindow const& window)
{
	m_window = window;
	m_window.PointerMoved({ this, &Graphics::OnMouseMove });
	Resize();
}

void Dx::Graphics::Resize()
{
	m_width = m_window.Bounds().Width;
	m_height = m_window.Bounds().Height;
	this->CreateWindowSizeDependentResources();
}

void Dx::Graphics::StartFrame(float color[4])
{
	ID3D11RenderTargetView* views[] = { m_renderTargetView.get() };
	m_context->OMSetRenderTargets(
		1,
		views,
		m_depthStencilView.get()
	);

	m_context->ClearRenderTargetView(m_renderTargetView.get(), color);
	m_context->ClearDepthStencilView(m_depthStencilView.get(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.f, 0);
}

float Dx::Graphics::Width()
{
	return m_width;
}

float Dx::Graphics::Height()
{
	return m_height;
}

float Dx::Graphics::MouseX()
{
	return m_mouseX;
}

float Dx::Graphics::MouseY()
{
	return m_mouseY;
}

void Dx::Graphics::Present()
{
	m_swapChain->Present(1, 0);
}

com_ptr<ID3D11Device3> Dx::Graphics::Device()
{
	return m_device;
}

com_ptr<ID3D11DeviceContext4> Dx::Graphics::Context()
{
	return m_context;
}

CoreWindow Dx::Graphics::Window()
{
	return m_window;
}
