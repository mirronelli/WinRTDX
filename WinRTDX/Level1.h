#pragma once
#include "pch.h"
#include "ILevel.h"
#include "VertexShader.h"
#include "PixelShader.h"

namespace Dx::Levels
{
	class Level1 : public ILevel
	{

		struct VertexSimple
		{
			float x, y, z, w; // vertex position
			float r, g, b, a; // vertex color
		};

	public:
		using ILevel::ILevel;
		concurrency::task<void> Load();
		void SetupModel();
		void Render();
		void Update(float delta);
	
	private:
		com_ptr<ID3D11Buffer>	mVertexBuffer;
		com_ptr<ID3D11Buffer>	mIndexBuffer;

		std::shared_ptr<Dx::Attachables::VertexShader>		mVertexShader;
		std::shared_ptr<Dx::Attachables::PixelShader>		mPixelShader;

		float							m_red{ 0 }, m_green{ 0 }, m_blue{ 0 };
		float							m_elapsedTime{ 0 };
		float							m_effectDuration{ 10.0f };

		const float					c_oneSixth{ 1.0f / 6.0f };
		const float					c_twoSixths{ 2.0f / 6.0f };
		const float					c_threeSixths{ 3.0f / 6.0f };
		const float					c_fourSixths{ 4.0f / 6.0f };
		const float					c_fiveSixths{ 5.0f / 6.0f };
		const float					c_sixSixths{ 6.0f / 6.0f };
		const float					c_maxColorIntensity{ 0.2f };
	};

}