#pragma once
#include "pch.h"
#include "Structures.h"

namespace Dx::Attachables
{
	enum class ResourceSlots
	{
		PerLevel = 0,
		PerFrame = 1,
		PerInstance = 2
	};

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
		static inline std::string mCurrentTexture;
		static inline Dx::Drawables::VertexType mCurrentInputLayout;
		static inline Dx::Drawables::VertexType mCurrentVertexShader;
		static inline Dx::Drawables::VertexType mCurrentPixelShader;
	};
}