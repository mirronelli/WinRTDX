#include "pch.h"
#include "CacheWithPreload.h"
#include "VertexShader.h"
#include "PixelShader.h"

using namespace Dx::Drawables;

namespace Dx::Attachables
{
	template CacheWithPreload<Dx::Attachables::VertexShader>;
	template CacheWithPreload<Dx::Attachables::PixelShader>;

	template<class T>
	CacheWithPreload<T>::CacheWithPreload(VertexType key)
	{
		mKey = key;
	}

	template<class T>
	void CacheWithPreload<T>::Preload(Dx::Drawables::VertexType type, std::wstring fileName)
	{
		mMap[type] = std::make_shared<T>(type, fileName);
	}

	template <class T>
	std::shared_ptr<T> CacheWithPreload<T>::Get(VertexType type)
	{
		return mMap[type];
	}

	template<class T>
	bool CacheWithPreload<T>::IsCurrent(VertexType type)
	{
		return type == mCurrentType;
	}

	template<class T>
	void CacheWithPreload<T>::SetCurrent(VertexType type)
	{
		mCurrentType = type;
	}
}