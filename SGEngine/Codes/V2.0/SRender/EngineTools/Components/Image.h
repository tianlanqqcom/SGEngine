/*
*	Created by tianlan.
*	UI�����2DͼƬ
* 
*	class Image;
*/

#ifndef SGENGINE_IMAGE_H
#define SGENGINE_IMAGE_H
#include "../../MathTools/Matrix.h"
#include "../../RGBColor.h"
#include "../Vector3.h"
#include "../ResourceManager.h"
#include "../UIBase.h"
#include "../GameObject.h"
#include "../Component.h"
#include "Canvas.h"

namespace EngineTools 
{
	class Image : public Component, public UIBase
	{
		int tempPosX = 0;			// ��ʱλ�ü�¼�����������ж���һ֡UI�Ƿ����ƶ�
		int tempPosY = 0;			// ��ʱλ�ü�¼�����������ж���һ֡UI�Ƿ����ƶ�
		bool hasChange = true;		// ����Ƿ��б任
		Canvas* canvas = nullptr;	// ����ָ��

	public:
		Image();

		void Start() override;

		void Update() override;

		// ��ͼƬ
		// @para path	�ļ�·��
		// @para type	�ļ�����
		void BindImage(const std::string& path, ImageType type);

		// �޸����꣬�ƶ�����Ӧ����
		// @para position	�µ�����
		//		first:	x
		//		second: y
		void MoveTo(const std::pair<int, int>& position);

		// ƽ�ƶ�Ӧ����
		// @para vec		�ƶ�����
		//		first:	x
		//		second: y
		void Translate(const std::pair<int, int>& vec);

		// ����ͼƬ���µĴ�С
		// @para nHeight	�µĸ߶�
		// @para nWidth		�µĿ��
		void ScaleTo(int nHeight, int nWidth);

		// ���Ӧ������
		// @para pCanvas	Ҫ�󶨵Ļ���ָ��
		void BindCanvas(Canvas* pCanvas);

		// ���Ӧ·���Ļ�����
		// @para path	����������·��
		void BindCanvas(const std::string& path);

	};
}



#endif // !SGENGINE_IMAGE_H


