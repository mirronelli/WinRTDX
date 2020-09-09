#include "pch.h"
#include "DeviceResources.h"
#include <DxTools.h>


Dx::DeviceResources::DeviceResources()
{
	CreateDeviceResources();
};

void Dx::DeviceResources::CreateDeviceIndependentResources()
{

}

void Dx::DeviceResources::CreateDeviceResources()
{
	// This flag adds support for surfaces with a different color channel ordering
	// than the API default. It is required for compatibility with Direct2D.
	UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#if defined(_DEBUG)
	if (Dx::Tools::SdkLayersAvailable())
	{
		// If the project is in a debug build, enable debugging via SDK Layers with this flag.
		creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
	}
#endif

	// This array defines the set of DirectX hardware feature levels this app will support.
	// Note the ordering should be preserved.
	// Don't forget to declare your application's minimum required feature level in its
	// description. All applications are assumed to support 9.1 unless otherwise stated.
	D3D_FEATURE_LEVEL requestedFeatureLevels[] =
	{
		 D3D_FEATURE_LEVEL_11_1,
		 D3D_FEATURE_LEVEL_11_0
	};

	// Create the Direct3D 11 API device object and a corresponding context.
	winrt::com_ptr<ID3D11Device> device;
	winrt::com_ptr<ID3D11DeviceContext> context;
	winrt::com_ptr<IDXGIAdapter> adapter;
	D3D_FEATURE_LEVEL featureLevel;

	//adapter = Dx::Tools::GetPreferredAdapter();
	D3D11CreateDevice(
	   adapter.get(),									// Specify nullptr to use the default adapter.
	   D3D_DRIVER_TYPE_HARDWARE,					// Create a device using the hardware graphics driver.
	   0,													// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
	   creationFlags,									// Set debug and Direct2D compatibility flags.
		requestedFeatureLevels,						// List of feature levels this app can support.
	   ARRAYSIZE(requestedFeatureLevels),		// Size of the list above.
	   D3D11_SDK_VERSION,							// Always set this to D3D11_SDK_VERSION for Windows Runtime apps.
	   device.put(),									// Returns the Direct3D device created.
	   &featureLevel,									// Returns feature level of device created.
	   context.put()									// Returns the device immediate context.
	);

	m_d3dDevice = device.as<ID3D11Device3>();
	m_d3dContext = context.as<ID3D11DeviceContext4>();
	//// Create the Direct2D device object and a corresponding context.
	//winrt::com_ptr<IDXGIDevice3> dxgiDevice;
	//dxgiDevice = m_d3dDevice.as<IDXGIDevice3>();

	//winrt::check_hresult(
	//   m_d2dFactory->CreateDevice(dxgiDevice.get(), m_d2dDevice.put())
	//);

	//winrt::check_hresult(
	//   m_d2dDevice->CreateDeviceContext(
	//      D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
	//      m_d2dContext.put()
	//   )
	//);
}
