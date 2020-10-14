#pragma once

namespace Dx::Tools
{
	// Check for SDK Layer support.
	static inline bool SdkLayersAvailable()
	{
		HRESULT hr = S_OK;
		try 
		{
			hr = D3D11CreateDevice(
				nullptr,							 // Adapter
				D3D_DRIVER_TYPE_NULL,       // There is no need to create a real hardware device.
				0,									 // HModule
				D3D11_CREATE_DEVICE_DEBUG,  // Flags - Check for the SDK layers
				nullptr,                    // Requested feature levels array. 
				0,									 // Requested feature levels count
				D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
				nullptr,                    // No need to keep the D3D device reference.
				nullptr,                    // No need to know the feature level.
				nullptr                     // No need to keep the D3D device context reference.
			);
		}
		catch (...) {}

		return SUCCEEDED(hr);
	}
	static void DisplayAdapterDetails(winrt::com_ptr<IDXGIAdapter> adapter)
	{
		DXGI_ADAPTER_DESC adapterDesc;
		adapter->GetDesc(&adapterDesc);

		std::wostringstream debug;
		debug << "Found adapter:\t" << adapterDesc.Description << "\n";
		OutputDebugStringW(debug.str().c_str());
	}


	// Helper class for COM exceptions
	class com_exception : public std::exception
	{
	public:
		com_exception(HRESULT hr) : result(hr) {}

		const char* what() const override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	// Helper utility converts D3D API failures into exceptions.
	static inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr))
		{
			throw com_exception(hr);
		}
	}

	static inline bool ends_with(hstring const& value, hstring const& ending)
	{
		if (ending.size() > value.size()) return false;
		return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
	}
}

