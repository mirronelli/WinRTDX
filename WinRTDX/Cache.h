#pragma once

#include <map>
#include "Structures.h"

namespace Dx::Attachables
{

	template <class T>
	class Cache
	{
	public:
		static std::shared_ptr<T> Get(Dx::Drawables::VertexType type);
		static bool IsCurrent(Dx::Drawables::VertexType type);
		static void SetCurrent(Dx::Drawables::VertexType type);
	private:
		inline static std::map<Dx::Drawables::VertexType, std::shared_ptr<T>> mMap;
		inline static Dx::Drawables::VertexType mCurrentType;
	};

}
