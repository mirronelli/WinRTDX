#pragma once
#include "pch.h"
#include "ILevel.h"

namespace Dx::Levels
{

class Level1 : public ILevel
{
public:
	Level1(std::shared_ptr<Dx::Graphics> graphics) : ILevel(graphics) {};
	
	concurrency::task<void> Load();
	void Render();
	void Update(UINT32 deltaTime);
};

}