#pragma once
#include "StepTimer.h"
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "LevelBase.h"

class Game
{
public:
	Game(winrt::Windows::UI::Core::CoreWindow const & window);
	void Init();
	void LoadLevel(byte name);
	void Run();
	void Close();
	void Resize();

private:
	void ProcessEvents();
	void ProcessKeyboard();
	void Update(Dx::StepTimer const& timer);
	void Tick();
	void Render();
	void Present();

	std::unique_ptr<Dx::Levels::LevelBase>				mLevel{ nullptr };
	CoreWindow												mWindow{ nullptr };
	bool														mIsClosing = false;
	Dx::StepTimer											mTimer;
	ULONG64													mFrame = 0;

	byte														mMaxLevel = 9;
	byte														mCurrentLevel = 5;
	//byte														mCurrentLevel = mMaxLevel;
	bool														mStop = false;

	std::shared_ptr<Dx::KeyboardInput>				mKeyboardInput{ nullptr };
	std::shared_ptr<Dx::MouseInput>					mMouseInput{ nullptr };

};

