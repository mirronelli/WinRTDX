#include "pch.h"
#include "Cache.h"
#include "InputLayout.h"

using Dx::Drawables::VertexType;

namespace Dx::Attachables
{
	template Cache<VertexType, Dx::Attachables::InputLayout>;

	template<typename Tkey, typename Tvalue>
	Cache<Tkey, Tvalue>::Cache(Tkey key)
	{
		mKey = key;
	}

	template <typename Tkey, typename Tvalue>
	std::shared_ptr<Tvalue> Cache<Tkey, Tvalue>::Get(Tkey key)
	{
		std::shared_ptr<Tvalue> result = mMap[key];
		if (!result)
		{
			result = std::make_shared<Tvalue>(key);
			mMap[key] = result;
		}
		return result;
	}
	
	template<typename Tkey, typename Tvalue>
	bool Cache<Tkey, Tvalue>::IsCurrent(Tkey key)
	{
		return key == mCurrentKey;
	}

	template<typename Tkey, typename Tvalue>
	void Cache<Tkey, Tvalue>::SetCurrent(Tkey key)
	{
		mCurrentKey = key;
	}
}