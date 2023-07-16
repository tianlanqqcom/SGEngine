/*
*	Created by tianlan.
*	����������洢ͼƬ�ʲ�����
* 
*	class ImageAsset;
*/
#ifndef SGENGINE_IMAGEASSET_H
#define SGENGINE_IMAGEASSET_H

#include "../BMPLoader.h"
#include "DebugHelper.h"
#include "../ThirdParty/LodePNG/lodepng.h"

namespace EngineTools 
{
	// ͼƬ�ļ�����
	enum class ImageType
	{
		BMP,
		PNG,
		JPG
	};

	class ImageAsset
	{
	public:
		// �Ѿ�������ͼƬ�ʲ�����
		static unsigned int ImageCount;

		// ͼƬID
		unsigned int ImageID;

		// ԭʼͼƬ���
		int ImageWidth = 0;

		// ԭʼͼƬ�߶�
		int ImageHeight = 0;

		// ͼƬ�ļ�����
		ImageType Type = ImageType::BMP;

		// ԭʼͼƬ��������
		MathTools::Matrix<RGBAColor> ImgData;

		ImageAsset();

		ImageAsset(const std::string& path, ImageType type = ImageType::BMP);

		// ����ͼƬ���µĴ�С��ʹ��˫���Բ�ֵ����
		// @para height	�µĸ߶�
		// @para width	�µĿ��
		MathTools::Matrix<RGBAColor>* ScaleTo(int height, int width);

		// ˫���Բ�ֵ��
		// @para resImgData		���ͼƬ����������
		// @para scaleX			ͼƬ��X�����ϵ����ű���
		// @para scaleY			ͼƬ��Y�����ϵ����ű���
		// @para height			�µĸ߶�
		// @para width			�µĿ��
		// @para sourceHeight	ԭʼ�߶�
		// @para sourceWidth	ԭʼ���
		// @in	 ԭʼ����ֱ�Ӵӵ�ǰͼƬ�ʲ��л�ȡ
		void LinarInter(RGBAColor* resImgData, double scaleX, double scaleY, int height, int width, int sourceHeight, int sourceWidth);
	};
}

#endif // !SGENGINE_IMAGEASSET_H



