// ÑÝÊ¾ÎÄ¼þ
#pragma once
#include "Scene.h"

namespace EngineTools 
{
	class MyScene : public Scene
	{
	public:
		MyScene()
		{
			// Do Init Operation
		}

		~MyScene()
		{
			// If needed, free the memory
		}

		std::string ToString(int depth = 0) const override
		{
			return "This is MyScene";
		}
	};
}