//
// Created by tianlan on 2023/5/3.
//

#include "Light.h"

namespace EngineTools
{
    Light::Light()
    {
        typeHashCode = typeid(Light).hash_code();
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
} // EngineTools