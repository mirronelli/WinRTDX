#include "pch.h"
#include "Cache.h"
#include "InputLayout.h"

namespace Dx::Attachables
{
	template Cache<Dx::Attachables::InputLayout>;

	template <class T>
	std::shared_ptr<T> Cache<T>::Get(Dx::Drawables::VertexType type)
	{
		std::shared_ptr<T> result = mMap[type];
		if (!result)
		{
			result = std::make_shared<T>(type);
			mMap[type] = result;
		}
		return result;
	}
	
	template<class T>
	bool Cache<T>::IsCurrent(Dx::Drawables::VertexType type)
	{
		return type == mCurrentType;
	}

	template<class T>
	void Cache<T>::SetCurrent(Dx::Drawables::VertexType type)
	{
		mCurrentType = type;
	}
}