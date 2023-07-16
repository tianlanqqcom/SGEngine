#include "ResourceManager.h"

using namespace EngineTools;

EngineTools::ResourceManager* EngineTools::ResourceManager::instance = nullptr;

// �����������ͷ��б����������Դ
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

// ��ȡʵ��ָ��

EngineTools::ResourceManager* EngineTools::ResourceManager::GetInstance()
{
	if (!instance)
	{
		instance = new ResourceManager();
	}
	return instance;
}

// �ͷ�ʵ���������Ѽ��ص�������Դ

void EngineTools::ResourceManager::DestoryInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

// ������ָ��·����ͼƬ�ʲ�
// @para path			�ļ�·��
// @para imgType		ͼƬ����
// @ret  ImageAsset*	���س�����ͼƬ�ʲ�ָ��

ImageAsset* EngineTools::ResourceManager::LoadImageAtPath(const std::string& path, ImageType imgType)
{
	// ���Ҹ��ʲ��Ƿ��Ѿ����أ�����Ѽ��أ�ֱ�ӷ���
	auto ret = FindImageAsset(path);
	if (ret)
	{
		return ret;
	}

	ImageAsset* res = nullptr;

	// ����ͼƬ
	res = new ImageAsset(path, imgType);

	// ���ͼƬ���سɹ���ע�����Դ
	if (res->ImgData.GetCol() > 1)
	{
		RegisterImageAsset(path, res);
	}
	return res;
}

// ע��ͼƬ�ʲ�
// @para path	�ļ�·��
// @para asset	�ʲ�ָ��

void EngineTools::ResourceManager::RegisterImageAsset(const std::string& path, ImageAsset* asset)
{
	imgs[path] = asset;
}

// ����ͼƬ�ʲ�
// @para path			�ļ�·��
// @ret  ImageAsset*	�ʲ�ָ��

ImageAsset* EngineTools::ResourceManager::FindImageAsset(const std::string& path)
{
	auto it = imgs.find(path);
	if (it != imgs.end())
	{
		return it->second;
	}
	return nullptr;
}

// ���ض�Ӧ·��������ģ��
// @para path	�ļ�·��
// @para type	�ļ�����
// @ret  int	ģ���ʲ���UID

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

	// ���ģ�ͼ��سɹ���ע��ģ��
	if (ret)
	{
		RegisterModelAsset(ret->modelID, ret);
		return ret->modelID;
	}
	return -1;
}

// ע��ģ���ʲ�
// @para modelID	ģ��UID
// @para asset		�ʲ�ָ��

void EngineTools::ResourceManager::RegisterModelAsset(int modelID, ModelAsset* asset)
{
	models[modelID] = asset;
}

// ����ͼƬ�ʲ�
// @para path			�ļ�·��
// @ret  ImageAsset*	�ʲ�ָ��

ModelAsset* EngineTools::ResourceManager::FindModelAsset(int modelID)
{
	auto it = models.find(modelID);
	if (it != models.end())
	{
		return it->second;
	}
	return nullptr;
}
