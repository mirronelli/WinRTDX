#pragma once
#include "pch.h"
#include "ILevel.h"

namespace Dx::Levels
{

	class Level1 : public ILevel
	{
		struct Vertex
		{
			float x, y, z;    // vertex position
		};

	public:
		Level1(std::shared_ptr<Dx::Graphics> graphics) : ILevel(graphics) {};

		concurrency::task<void> Load();
		void SetupModel();
		void Render();
		void Update(UINT32 deltaTime);
	
	private:
		com_ptr<ID3D11Buffer>	m_vertexBuffer;
		IBuffer						m_compiledVertexShader{ nullptr };
		IBuffer						m_compiledPixelShader{ nullptr };
	};

}