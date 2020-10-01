#pragma once
#include "KeyboardInput.h"
#include "MouseInput.h"

namespace Dx::Levels
{

	class ILevel
	{
	public:
		ILevel(std::shared_ptr<KeyboardInput> keyboardInput, std::shared_ptr<MouseInput> mouseInput) :
			m_keyboardInput(keyboardInput),
			m_mouseInput(mouseInput)
		{};

		virtual concurrency::task<void> Load() = 0;
		virtual void SetupModel() = 0;
		virtual void Render() = 0;
		virtual void Update(float delta) = 0;

		virtual ~ILevel() {}
	protected:
		std::shared_ptr<KeyboardInput>   m_keyboardInput;
		std::shared_ptr<MouseInput>      m_mouseInput;
	};

}