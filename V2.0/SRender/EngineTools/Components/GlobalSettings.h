// 
// Created by tianlan.
// 全局设置，用于在一个场景中的不同物体间分享数据，也可以在场景间分享数据
//
// 更新日志：
// V1.0 -> V2.0:
// 添加了对光照、摄像机和物体信息的收集


#ifndef SGENGINE_GLOBALSETTINGS
#define SGENGINE_GLOBALSETTINGS

#include "../ResourceManager.h"
#include "../Component.h"
#include "Camera.h"
#include "Light.h"

namespace EngineTools
{
	class RenderInfo;

	class GlobalSettings : public Component
	{
	public:
		GlobalSettings();

		const std::string EngineVersion = "2.0";	// 当前引擎版本2.0

		std::map<const RenderInfo*, ModelAsset*> modelMap;		// 模型信息
		std::map<int, Camera*> cameras;							// 摄像机信息
		std::map<LightType, std::vector<Light*>> orderedLights;	// 光照信息

		std::string ToString(int depth = 0) const override;

		void Start() override;
	};
}

#endif
