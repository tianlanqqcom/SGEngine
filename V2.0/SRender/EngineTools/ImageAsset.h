/*
*	Created by tianlan.
*	这个类用来存储图片资产数据
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
	// 图片文件类型
	enum class ImageType
	{
		BMP,
		PNG,
		JPG
	};

	class ImageAsset
	{
	public:
		// 已经创建的图片资产个数
		static unsigned int ImageCount;

		// 图片ID
		unsigned int ImageID;

		// 原始图片宽度
		int ImageWidth = 0;

		// 原始图片高度
		int ImageHeight = 0;

		// 图片文件类型
		ImageType Type = ImageType::BMP;

		// 原始图片像素数据
		MathTools::Matrix<RGBAColor> ImgData;

		ImageAsset();

		ImageAsset(const std::string& path, ImageType type = ImageType::BMP);

		// 缩放图片到新的大小（使用双线性插值法）
		// @para height	新的高度
		// @para width	新的宽度
		MathTools::Matrix<RGBAColor>* ScaleTo(int height, int width);

		// 双线性插值法
		// @para resImgData		输出图片像素数据组
		// @para scaleX			图片在X方向上的缩放比例
		// @para scaleY			图片在Y方向上的缩放比例
		// @para height			新的高度
		// @para width			新的宽度
		// @para sourceHeight	原始高度
		// @para sourceWidth	原始宽度
		// @in	 原始数据直接从当前图片资产中获取
		void LinarInter(RGBAColor* resImgData, double scaleX, double scaleY, int height, int width, int sourceHeight, int sourceWidth);
	};
}

#endif // !SGENGINE_IMAGEASSET_H



