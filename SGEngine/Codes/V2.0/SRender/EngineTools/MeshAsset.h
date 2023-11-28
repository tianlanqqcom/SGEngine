/*
*	Created by tianlan.
*	��������Ϣ�������븸ģ�͹��������ҵ�������Ϣ
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
	class ModelAsset;	// ǰ������

	class MeshAsset
	{
	public:
		// ����������
		std::string meshName;

		// ��������
		std::vector<Vector3> vertex;

		// ������������
		std::vector<std::pair<float, float>> vertexTexture;

		// ���㷨��
		std::vector<Vector3> vertexNormal;

		// ���б�
		std::vector<FaceAsset> faces;

		// ��ģ��ָ��
		ModelAsset* parentModel = nullptr;
	};
}
#endif // !SGENGINE_MESHASSET_H



