#pragma once
#include "KeyboardInput.h"
#include "MouseInput.h"
#include "Attachable.h"

namespace Dx::Levels
{

	class LevelBase
	{
	public:
		LevelBase(std::shared_ptr<KeyboardInput> keyboardInput, std::shared_ptr<MouseInput> mouseInput) :
			mKeyboardInput(keyboardInput),
			mMouseInput(mouseInput)
		{};

		virtual concurrency::task<void> Load() = 0;
		virtual void Render() = 0;
		virtual void Update(float delta) = 0;

		virtual void Start() {};

		virtual ~LevelBase() {}
	protected:
		std::shared_ptr<KeyboardInput>   mKeyboardInput;
		std::shared_ptr<MouseInput>      mMouseInput;
	};

}