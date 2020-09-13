#pragma once

#include "Graphics.h"

namespace Dx::Levels
{

class ILevel
{
public:
   ILevel(std::shared_ptr<Dx::Graphics> graphics) :m_graphics(graphics) {};

   virtual concurrency::task<void> Load() = 0;
   virtual void Render() = 0;
   virtual void Update(UINT32 deltaTime) = 0;

   virtual ~ILevel() {}
protected:
   std::shared_ptr<Dx::Graphics> m_graphics;
};

}