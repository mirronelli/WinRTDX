#include "pch.h"
#include <Game.h>

using namespace winrt;
using namespace Windows;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation; 
using namespace Windows::Graphics::Display;
using namespace Windows::UI;
using namespace Windows::UI::Core;
using namespace Windows::UI::Composition;

struct App : implements<App, IFrameworkViewSource, IFrameworkView>
{
	IFrameworkView CreateView()
	{
		return *this;
	}

	void Initialize(CoreApplicationView const& applicationView)
	{
		applicationView.Activated({ this, &App::OnActivated });
	}

	void SetWindow(CoreWindow const& window)
	{
		mWindow = window;
		mWindow.Closed({ this, &App::OnWindowClosed });
		mWindow.SizeChanged({ this, &App::OnWindowSizeChanged });
	}

	void Load(hstring const&)
	{
		if (!mGame)
		{
			mGame = std::make_unique<Game>(mWindow);
			mGame->Init();
		}
	}

	void Uninitialize()
	{
		mGame->Close();
	}

	void Run()
	{
		mGame->Run();
	}

#pragma region Events

	void OnActivated(CoreApplicationView const& /* applicationView */, IActivatedEventArgs const& /* args */)
	{
		CoreWindow window = CoreWindow::GetForCurrentThread();
		window.Activate();
	}

	void OnWindowClosed(CoreWindow const& /* sender */, CoreWindowEventArgs const& /* args */)
	{
		mGame->Close();
	}
	
	void OnWindowSizeChanged(CoreWindow const& /* window */, WindowSizeChangedEventArgs const& /* args */)
	{
		mGame->Resize();
	}

	void OnDpiChanged(DisplayInformation const& /* sender */, IInspectable const& /* args */)
	{
		mGame->Resize();
	}

	
#pragma endregion

private:
	std::unique_ptr<Game>	mGame;
	CoreWindow					mWindow{ nullptr };
};

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
	CoreApplication::Run(make<App>());
}
