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
		virtual void Attach() = 0;
		static void Reset()
		{
			mCurrentIndexBuffer = "";
			mCurrentVertexBuffer = "";
			mCurrentTexture = "";
			mCurrentInputLayout = Dx::Drawables::VertexType::None;
			mCurrentPixelShader = Dx::Drawables::VertexType::None;
			mCurrentVertexShader = Dx::Drawables::VertexType::None;
		}
	protected:
		static inline std::string mCurrentVertexBuffer;
		static inline std::string mCurrentIndexBuffer;
		static inline std::string mCurrentTexture;
		static inline Dx::Drawables::VertexType mCurrentInputLayout;
		static inline Dx::Drawables::VertexType mCurrentVertexShader;
		static inline Dx::Drawables::VertexType mCurrentPixelShader;
	};
}