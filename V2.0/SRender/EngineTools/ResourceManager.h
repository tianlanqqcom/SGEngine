/*
*	Created by tianlan.
*	���������������Դ��������Ŀǰֻ�洢��ImageAsset��ModelAsset
* 
*	������
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
		// �Ѿ����ص�����ͼƬ
		std::map<std::string, ImageAsset*> imgs;

		// �Ѿ����ص�����ģ��
		std::map<int, ModelAsset*> models;

		// ָ���Լ�ʵ����ָ��
		static ResourceManager* instance;

		// Ĭ�Ϲ��캯��
		ResourceManager() = default;

		// �����������ͷ��б����������Դ
		~ResourceManager();
	public:
		// ��ȡʵ��ָ��
		static ResourceManager* GetInstance();

		// �ͷ�ʵ���������Ѽ��ص�������Դ
		static void DestoryInstance();

		// ������ָ��·����ͼƬ�ʲ�
		// @para path			�ļ�·��
		// @para imgType		ͼƬ����
		// @ret  ImageAsset*	���س�����ͼƬ�ʲ�ָ��
		ImageAsset* LoadImageAtPath(const std::string& path, ImageType imgType = ImageType::BMP);

		// ע��ͼƬ�ʲ�
		// @para path	�ļ�·��
		// @para asset	�ʲ�ָ��
		void RegisterImageAsset(const std::string& path, ImageAsset* asset);

		// ����ͼƬ�ʲ�
		// @para path			�ļ�·��
		// @ret  ImageAsset*	�ʲ�ָ��
		ImageAsset* FindImageAsset(const std::string& path);

		// ���ض�Ӧ·��������ģ��
		// @para path	�ļ�·��
		// @para type	�ļ�����
		// @ret  int	ģ���ʲ���UID
		int LoadObjectAtPath(const std::string& path, ModelType type = ModelType::OBJ);

		// ע��ģ���ʲ�
		// @para modelID	ģ��UID
		// @para asset		�ʲ�ָ��
		void RegisterModelAsset(int modelID, ModelAsset* asset);

		// ����ͼƬ�ʲ�
		// @para path			�ļ�·��
		// @ret  ImageAsset*	�ʲ�ָ��
		ModelAsset* FindModelAsset(int modelID);
	};
}
#endif // !SGENGINE_RESOURCEMANAGER_H



