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
		static void CreateInstance();
		Graphics();

		void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window);
		void Resize();

		void StartFrame(float color[4]);
		void Present();

		inline static std::shared_ptr<Graphics>		Instance;
		inline static com_ptr<ID3D11Device3>			Device;
		inline static com_ptr<ID3D11DeviceContext4>	Context;
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
		com_ptr<IDXGIFactory7>						mFactory;
		com_ptr<IDXGISwapChain4>					mSwapChain;
		com_ptr<ID3D11RenderTargetView>			mRenderTargetView;
		com_ptr<ID3D11DepthStencilView>			mDepthStencilView;

		// Windows objects
		CoreWindow										mWindow{ nullptr };
		float												mWidth;
		float												mHeight;
		float												mDpi = 1;
		float												mMouseX{ 0 };
		float												mMouseY{ 0 };
	};
}