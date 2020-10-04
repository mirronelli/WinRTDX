#pragma once

#include <map>
#include "Structures.h"

namespace Dx::Attachables
{

	template <typename Tkey, typename Tvalue>
	class Cache
	{
	public:
		Cache(Tkey key);
		static std::shared_ptr<Tvalue> Get(Tkey key);
		static bool IsCurrent(Tkey key);
		static void SetCurrent(Tkey key);
	private:
		inline static std::map<Tkey, std::shared_ptr<Tvalue>> mMap;
		inline static Tkey mCurrentKey;

	protected:
		Tkey	mKey;
	};

}
