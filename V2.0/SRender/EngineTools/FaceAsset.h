/*
*	Created by tianlan.
*	存储面数据
* 
*	class FaceAsset;
*/
#ifndef SGENGINE_FACEASSET_H
#define SGENGINE_FACEASSET_H
#include <string>

namespace EngineTools 
{
	class FaceAsset
	{
	public:
		// 关联的材质名称
		std::string connectedMaterialName;

		// 顶点索引
		int vertexIndex[3];

		// 顶点纹理索引
		int vertexTextureIndex[3];

		// 顶点法线索引
		int vertexNormalIndex[3];
	};
}
#endif // !SGENGINE_FACEASSET_H



