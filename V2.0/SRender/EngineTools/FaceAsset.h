/*
*	Created by tianlan.
*	�洢������
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
		// �����Ĳ�������
		std::string connectedMaterialName;

		// ��������
		int vertexIndex[3];

		// ������������
		int vertexTextureIndex[3];

		// ���㷨������
		int vertexNormalIndex[3];
	};
}
#endif // !SGENGINE_FACEASSET_H



