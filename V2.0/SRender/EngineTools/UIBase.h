/*
*	Created by tianlan.
*	��������UI����Ļ���
* 
*	class UIBase;
*/

#ifndef SGENGINE_UIBASE_H
#define SGENGINE_UIBASE_H

#include "ResourceManager.h"
#include "../MathTools/Matrix.h"

namespace EngineTools 
{
	class UIBase
	{
	public:
		int posX;		// UI��X����
		int posY;		// UI��Y����
		int width;		// UI�Ŀ��
		int height;		// UI�ĸ߶�
		float rotation; // UI����תֵ������ʱ��Ϊ����ע�⣺���ֵ��ʱ��û��

		// ��UI�Ƿ��Ѿ��ڻ���ע��
		bool isRegistered = false;

		// UIͼ���ԭʼ����
		ImageAsset* imgData = nullptr;

		// �������������������
		SRender::MathTools::Matrix<RGBAColor>* out = nullptr;
	};
}
#endif // !SGENGINE_UIBASE_H



