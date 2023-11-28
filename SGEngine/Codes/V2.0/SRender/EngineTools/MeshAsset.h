/*
*	Created by tianlan.
*	网格体信息，必须与父模型关联才能找到材质信息
* 
*	class MeshAsset;
*/
#ifndef SGENGINE_MESHASSET_H
#define SGENGINE_MESHASSET_H

#include <vector>
#include "Vector3.h"
#include "FaceAsset.h"

namespace EngineTools
{
	class ModelAsset;	// 前置声明

	class MeshAsset
	{
	public:
		// 网格体名称
		std::string meshName;

		// 顶点坐标
		std::vector<Vector3> vertex;

		// 顶点纹理坐标
		std::vector<std::pair<float, float>> vertexTexture;

		// 顶点法线
		std::vector<Vector3> vertexNormal;

		// 面列表
		std::vector<FaceAsset> faces;

		// 父模型指针
		ModelAsset* parentModel = nullptr;
	};
}
#endif // !SGENGINE_MESHASSET_H



