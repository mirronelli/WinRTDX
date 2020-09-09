#pragma once

namespace Dx
{
	class DeviceResources
	{

	public:
		DeviceResources();

	private:
		void CreateDeviceIndependentResources();
		void CreateDeviceResources();

		// Direct3D objects.
		D3D_FEATURE_LEVEL							  m_d3dFeatureLevel;
		winrt::com_ptr<ID3D11Device3>         m_d3dDevice; 
		winrt::com_ptr<ID3D11DeviceContext4>  m_d3dContext;
		winrt::com_ptr<IDXGISwapChain4>       m_swapChain;
	};
}