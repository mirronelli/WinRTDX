#pragma once
#include <DeviceResources.h>
#include "winrt/Windows.UI.Core.h";

class Game
{
public:
	Game(std::shared_ptr<Dx::DeviceResources> const& deviceResources, winrt::Windows::UI::Core::CoreWindow const & window);
	void Run();
	void Close();

private:
	void Update();
	void Render();
	void Present();

	std::shared_ptr<Dx::DeviceResources> m_deviceResources;
	winrt::Windows::UI::Core::CoreWindow m_parentWindow{ nullptr };
	bool m_isClosing = false;
};

