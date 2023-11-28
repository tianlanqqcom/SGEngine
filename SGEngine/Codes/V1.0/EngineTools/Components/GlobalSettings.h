// 
// Created by tianlan.
// 全局设置，用于在一个场景中的不同物体间分享数据，也可以在场景间分享数据

#ifndef SGENGINE_GLOBALSETTINGS
#define SGENGINE_GLOBALSETTINGS
#include "../Component.h"

namespace EngineTools
{
	class GlobalSettings : public Component
	{
	public:
		const std::string EngineVersion = "1.0";	// 当前引擎版本

		std::string ToString(int depth = 0) const override;
	};
}

#endif
