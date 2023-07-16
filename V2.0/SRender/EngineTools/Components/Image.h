/*
*	Created by tianlan.
*	UI组件，2D图片
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
		int tempPosX = 0;			// 临时位置记录变量，用于判断上一帧UI是否有移动
		int tempPosY = 0;			// 临时位置记录变量，用于判断上一帧UI是否有移动
		bool hasChange = true;		// 标记是否有变换
		Canvas* canvas = nullptr;	// 画布指针

	public:
		Image();

		void Start() override;

		void Update() override;

		// 绑定图片
		// @para path	文件路径
		// @para type	文件类型
		void BindImage(const std::string& path, ImageType type);

		// 修改坐标，移动至对应坐标
		// @para position	新的坐标
		//		first:	x
		//		second: y
		void MoveTo(const std::pair<int, int>& position);

		// 平移对应距离
		// @para vec		移动距离
		//		first:	x
		//		second: y
		void Translate(const std::pair<int, int>& vec);

		// 缩放图片到新的大小
		// @para nHeight	新的高度
		// @para nWidth		新的宽度
		void ScaleTo(int nHeight, int nWidth);

		// 与对应画布绑定
		// @para pCanvas	要绑定的画布指针
		void BindCanvas(Canvas* pCanvas);

		// 与对应路径的画布绑定
		// @para path	画布的物体路径
		void BindCanvas(const std::string& path);

	};
}



#endif // !SGENGINE_IMAGE_H


