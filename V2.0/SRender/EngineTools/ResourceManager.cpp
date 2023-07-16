#include "ResourceManager.h"

using namespace EngineTools;

EngineTools::ResourceManager* EngineTools::ResourceManager::instance = nullptr;

// 析构函数，释放列表里的所有资源
EngineTools::ResourceManager::~ResourceManager()
{
	for (auto& img : imgs)
	{
		delete img.second;
	}

	for (auto& model : models)
	{
		delete model.second;
	}
}

// 获取实例指针

EngineTools::ResourceManager* EngineTools::ResourceManager::GetInstance()
{
	if (!instance)
	{
		instance = new ResourceManager();
	}
	return instance;
}

// 释放实例并清理已加载的所有资源

void EngineTools::ResourceManager::DestoryInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

// 加载在指定路径的图片资产
// @para path			文件路径
// @para imgType		图片类型
// @ret  ImageAsset*	加载出来的图片资产指针

ImageAsset* EngineTools::ResourceManager::LoadImageAtPath(const std::string& path, ImageType imgType)
{
	// 查找该资产是否已经加载，如果已加载，直接返回
	auto ret = FindImageAsset(path);
	if (ret)
	{
		return ret;
	}

	ImageAsset* res = nullptr;

	// 加载图片
	res = new ImageAsset(path, imgType);

	// 如果图片加载成功，注册该资源
	if (res->ImgData.GetCol() > 1)
	{
		RegisterImageAsset(path, res);
	}
	return res;
}

// 注册图片资产
// @para path	文件路径
// @para asset	资产指针

void EngineTools::ResourceManager::RegisterImageAsset(const std::string& path, ImageAsset* asset)
{
	imgs[path] = asset;
}

// 查找图片资产
// @para path			文件路径
// @ret  ImageAsset*	资产指针

ImageAsset* EngineTools::ResourceManager::FindImageAsset(const std::string& path)
{
	auto it = imgs.find(path);
	if (it != imgs.end())
	{
		return it->second;
	}
	return nullptr;
}

// 加载对应路径的物体模型
// @para path	文件路径
// @para type	文件类型
// @ret  int	模型资产的UID

int EngineTools::ResourceManager::LoadObjectAtPath(const std::string& path, ModelType type)
{
	ModelAsset* ret = nullptr;
	switch (type)
	{
	case EngineTools::ModelType::OBJ:
		ret = ObjLoader::LoadObj(path);
		break;
	case EngineTools::ModelType::FBX:
		break;
	default:
		break;
	}

	// 如果模型加载成功，注册模型
	if (ret)
	{
		RegisterModelAsset(ret->modelID, ret);
		return ret->modelID;
	}
	return -1;
}

// 注册模型资产
// @para modelID	模型UID
// @para asset		资产指针

void EngineTools::ResourceManager::RegisterModelAsset(int modelID, ModelAsset* asset)
{
	models[modelID] = asset;
}

// 查找图片资产
// @para path			文件路径
// @ret  ImageAsset*	资产指针

ModelAsset* EngineTools::ResourceManager::FindModelAsset(int modelID)
{
	auto it = models.find(modelID);
	if (it != models.end())
	{
		return it->second;
	}
	return nullptr;
}
