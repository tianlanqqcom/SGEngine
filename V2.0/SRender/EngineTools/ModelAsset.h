/*
*	Created by tianlan.
*	这里是模型资产类
* 
*	class ModelAsset;
*/

#ifndef SGENGINE_MODELASSET_H
#define SGENGINE_MODELASSET_H
#include <map>
#include "MaterialAsset.h"
#include "MeshAsset.h"

namespace EngineTools
{
	// 模型文件类型
	enum class ModelType
	{
		OBJ,
		FBX
	};

	class ModelAsset
	{
	public:
		static int modelCount;	// 已创建的模型数量
		int modelID;			// 模型ID
		ModelType type;			// 文件类型

		// 材质列表
		std::map<std::string, MaterialAsset*> materials;
		// 网格体列表
		std::map<std::string, MeshAsset*> meshes;

		// 构造函数
		ModelAsset()
		{
			modelID = modelCount++;
			type = ModelType::OBJ;
		}

		ModelAsset(ModelType modelType)
		{
			modelID = modelCount++;
			type = modelType;
		}
	};


}
#endif // !SGENGINE_MODELASSET_H


