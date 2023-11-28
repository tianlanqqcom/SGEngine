/*
*	Created by tianlan.
*	�������������.obj�ļ��Ͷ�Ӧ��.mtl�ļ�
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
		// ��������������ģ��
		// @para path			�ļ�·��
		// @ret  ModelAsset*	ģ���ʲ���ָ��
		static ModelAsset* LoadObj(const std::string& path);

	private:
		static MeshAsset* nowOperatingMesh;				// ���ڲ������������ָ��
		static MaterialAsset* nowOperatingMaterial;		// ���ڲ����Ĳ����ʲ���ָ��
		static std::string nowUsingMaterialName;		// ����ʹ�õĲ�����

		// ��ģ����ָ������Ĳ���
		// @para model	ģ��ָ��
		// @para cmd	�Ѿ��ָ�õ�����
		static void DoOperation(ModelAsset* model, std::vector<std::string> cmd);

		// ��ȡ��Ӧ�Ĳ����ļ�
		// @para path	�ļ�·��
		// @para model	ģ��ָ��
		static void ReadMaterial(const std::string& path, ModelAsset* model);

		// MTL�ļ��еĲ���
		// @para model	ģ��ָ��
		// @para cmd	�Ѿ��ָ�õ�����
		static void MaterialOperation(ModelAsset* model, std::vector<std::string> cmd);
	};
}
#endif // !SGENGINE_OBJLOADER_H



