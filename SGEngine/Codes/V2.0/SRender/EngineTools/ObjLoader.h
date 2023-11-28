/*
*	Created by tianlan.
*	这个类用来加载.obj文件和对应的.mtl文件
*/

#ifndef SGENGINE_OBJLOADER_H
#define SGENGINE_OBJLOADER_H

#include <fstream>
#include "Vector3.h"
#include "ModelAsset.h"
#include "StringTools.h"

namespace EngineTools 
{
	class ObjLoader
	{
	public:
		// 公共函数：加载模型
		// @para path			文件路径
		// @ret  ModelAsset*	模型资产的指针
		static ModelAsset* LoadObj(const std::string& path);

	private:
		static MeshAsset* nowOperatingMesh;				// 正在操作的网格体的指针
		static MaterialAsset* nowOperatingMaterial;		// 正在操作的材质资产的指针
		static std::string nowUsingMaterialName;		// 正在使用的材质名

		// 对模型做指定命令的操作
		// @para model	模型指针
		// @para cmd	已经分割好的命令
		static void DoOperation(ModelAsset* model, std::vector<std::string> cmd);

		// 读取对应的材质文件
		// @para path	文件路径
		// @para model	模型指针
		static void ReadMaterial(const std::string& path, ModelAsset* model);

		// MTL文件中的操作
		// @para model	模型指针
		// @para cmd	已经分割好的命令
		static void MaterialOperation(ModelAsset* model, std::vector<std::string> cmd);
	};
}
#endif // !SGENGINE_OBJLOADER_H



