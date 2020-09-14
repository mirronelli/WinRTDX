#pragma once
#include "pch.h"
#include "ILevel.h"

namespace Dx::Levels
{

	class Level1 : public ILevel
	{
		struct Vertex
		{
			float x, y, z, w; // vertex position
			float r, g, b, a; // vertex color
		};

	public:
		Level1(std::shared_ptr<Dx::Graphics> graphics) : ILevel(graphics) {};

		concurrency::task<void> Load();
		void SetupModel();
		void Render();
		void Update(float delta);
	
	private:
		com_ptr<ID3D11Buffer>	m_vertexBuffer;
		IBuffer						m_compiledVertexShader{ nullptr };
		IBuffer						m_compiledPixelShader{ nullptr };
		float							m_red{ 0 }, m_green{ 0 }, m_blue{ 0 };
		float							m_elapsedTime{ 0 };
		float							m_effectDuration{ 1.0f };

		const float					c_oneSixth{ 1.0 / 6.0 };
		const float					c_twoSixths{ 2.0 / 6.0 };
		const float					c_threeSixths{ 3.0 / 6.0 };
		const float					c_fourSixths{ 4.0 / 6.0 };
		const float					c_fiveSixths{ 5.0 / 6.0 };
		const float					c_sixSixths{ 6.0 / 6.0 };
		const float					c_maxColorIntensity{ 0.1f };
	};

}