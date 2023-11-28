/*
*	Created by tianlan.
*	这是所有UI组件的基类
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
		int posX;		// UI的X坐标
		int posY;		// UI的Y坐标
		int width;		// UI的宽度
		int height;		// UI的高度
		float rotation; // UI的旋转值，以逆时针为正，注意：这个值暂时还没用

		// 该UI是否已经在画布注册
		bool isRegistered = false;

		// UI图像的原始数据
		ImageAsset* imgData = nullptr;

		// 输出给画布的像素数据
		SRender::MathTools::Matrix<RGBAColor>* out = nullptr;
	};
}
#endif // !SGENGINE_UIBASE_H



