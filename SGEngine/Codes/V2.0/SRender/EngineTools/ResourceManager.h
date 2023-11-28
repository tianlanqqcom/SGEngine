/*
*	Created by tianlan.
*	这是整个引擎的资源管理器，目前只存储了ImageAsset和ModelAsset
* 
*	单例类
*	class ResourceManager;
*/

#ifndef SGENGINE_RESOURCEMANAGER_H
#define SGENGINE_RESOURCEMANAGER_H

#include <map>
#include <string>
#include "ImageAsset.h"
#include "ModelAsset.h"
#include "ObjLoader.h"

namespace EngineTools
{
	class ResourceManager
	{
		// 已经加载的所有图片
		std::map<std::string, ImageAsset*> imgs;

		// 已经加载的所有模型
		std::map<int, ModelAsset*> models;

		// 指向自己实例的指针
		static ResourceManager* instance;

		// 默认构造函数
		ResourceManager() = default;

		// 析构函数，释放列表里的所有资源
		~ResourceManager();
	public:
		// 获取实例指针
		static ResourceManager* GetInstance();

		// 释放实例并清理已加载的所有资源
		static void DestoryInstance();

		// 加载在指定路径的图片资产
		// @para path			文件路径
		// @para imgType		图片类型
		// @ret  ImageAsset*	加载出来的图片资产指针
		ImageAsset* LoadImageAtPath(const std::string& path, ImageType imgType = ImageType::BMP);

		// 注册图片资产
		// @para path	文件路径
		// @para asset	资产指针
		void RegisterImageAsset(const std::string& path, ImageAsset* asset);

		// 查找图片资产
		// @para path			文件路径
		// @ret  ImageAsset*	资产指针
		ImageAsset* FindImageAsset(const std::string& path);

		// 加载对应路径的物体模型
		// @para path	文件路径
		// @para type	文件类型
		// @ret  int	模型资产的UID
		int LoadObjectAtPath(const std::string& path, ModelType type = ModelType::OBJ);

		// 注册模型资产
		// @para modelID	模型UID
		// @para asset		资产指针
		void RegisterModelAsset(int modelID, ModelAsset* asset);

		// 查找图片资产
		// @para path			文件路径
		// @ret  ImageAsset*	资产指针
		ModelAsset* FindModelAsset(int modelID);
	};
}
#endif // !SGENGINE_RESOURCEMANAGER_H



