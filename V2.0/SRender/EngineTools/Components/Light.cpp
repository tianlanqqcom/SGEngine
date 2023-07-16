//
// Created by tianlan on 2023/5/3.
//

#include "Light.h"
#include "../GameObject.h"
#include "../Scene.h"
#include "GlobalSettings.h"

namespace EngineTools
{
	int Light::lightCount = 0;

    Light::Light()
    {
        typeHashCode = typeid(Light).hash_code();
		lightID = lightCount++;
    }
    std::string Light::ToString(int depth) const
    {     
		std::string ret = "";
		for (int i = 0; i < depth; i++)
		{
			ret += ' ';
		}
		ret += "-Component:Light\n";

		return ret;
    }

	void Light::Start()
	{
		RegisterToGlobalSettings();
	}

	void Light::RegisterToGlobalSettings()
	{
		// gameObject->scene->GetSharedComponent<GlobalSettings>()->lights[lightID] = this;  Unused Now
		gameObject->scene->GetSharedComponent<GlobalSettings>()->orderedLights[lightType].emplace_back(this);
	}
} // EngineTools