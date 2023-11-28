/*
*	Created by tianlan.
*	材质信息
* 
*	struct MaterialAsset;
*/

#ifndef SGENGINE_MATERIALASSET_H
#define SGENGINE_MATERIALASSET_H

#include <string>
#include "Vector3.h"

namespace EngineTools 
{
	struct MaterialAsset
	{
		// 材质名
		std::string materialName;

		Vector3 Ka;
		Vector3 Kd;
		Vector3 Ks;
		Vector3 Ke;

		float Ns;
		float Ni;
		float d;

		int illum;

		std::string map_Ka;
		std::string map_Kd;
		std::string map_Ks;
		std::string map_Ns;
		std::string map_d;
		std::string map_bump;
	};
}
#endif // !SGENGINE_MATERIALASSET_H



