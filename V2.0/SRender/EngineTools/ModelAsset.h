/*
*	Created by tianlan.
*	������ģ���ʲ���
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
	// ģ���ļ�����
	enum class ModelType
	{
		OBJ,
		FBX
	};

	class ModelAsset
	{
	public:
		static int modelCount;	// �Ѵ�����ģ������
		int modelID;			// ģ��ID
		ModelType type;			// �ļ�����

		// �����б�
		std::map<std::string, MaterialAsset*> materials;
		// �������б�
		std::map<std::string, MeshAsset*> meshes;

		// ���캯��
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


