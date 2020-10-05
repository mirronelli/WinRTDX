#pragma once
#include "pch.h"
#include "Structures.h"

namespace Dx::Attachables
{
	class Attachable
	{
	public:
		Attachable(){};
		Attachable(int key) : m_key(key){};
		void Attach(bool force = false) { AttachPrivate(force); };

	protected:
		virtual void AttachPrivate(bool force) = 0;
		int m_key;

		static inline std::string mCurrentVertexBuffer;
		static inline std::string mCurrentIndexBuffer;
		static inline Dx::Drawables::VertexType mCurrentInputLayout;
	};
}