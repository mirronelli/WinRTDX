#pragma once

namespace Dx::Tools
{
	// Check for SDK Layer support.
	inline bool SdkLayersAvailable()
	{
		HRESULT hr = D3D11CreateDevice(
			nullptr,
			D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
			0,
			D3D11_CREATE_DEVICE_DEBUG,  // Check for the SDK layers.
			nullptr,                    // Any feature level will do.
			0,
			D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
			nullptr,                    // No need to keep the D3D device reference.
			nullptr,                    // No need to know the feature level.
			nullptr                     // No need to keep the D3D device context reference.
		);

		return SUCCEEDED(hr);
	}

	winrt::com_ptr<IDXGIAdapter4> GetPreferredAdapter(winrt::com_ptr<IDXGIFactory7> dxgiFactory)
	{
		winrt::com_ptr<IDXGIAdapter4> adapter;
		dxgiFactory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, __uuidof(IDXGIAdapter4), adapter.put_void());

		return adapter;
	}

	void DisplayAdapterDetails(winrt::com_ptr<IDXGIAdapter4> adapter)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		adapter->GetDesc(&adapterDesc);

		std::wostringstream debug;
		debug << "Found adapter:\t" << adapterDesc.Description << "\n";
		OutputDebugStringW(debug.str().c_str());
	}
}

