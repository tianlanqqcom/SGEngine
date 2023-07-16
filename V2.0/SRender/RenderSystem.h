/*
*	Created by tianlan.
*	这里是渲染系统主要控制部分，收集世界中所有画布及物体信息
*	绘制操作是高性能消耗单位，请尽量减少绘制画布的数量和次数
* 
*	单例类：
*	class RenderSystem;
*/

#ifndef SRENDER_RENDERSYSTEM
#define SRENDER_RENDERSYSTEM

#include <ctime>
#include "framework.h"
#include "MathTools/Matrix.h"
#include "RGBColor.h"
#include "Layer.h"
#include "Shader.h"
#include "EngineTools/Components/Canvas.h"

namespace EngineTools 
{
	class Canvas;	// 前置声明
}

namespace SRender
{
	class RenderSystem
	{
	private:
		RenderSystem() = default;					// 默认构造函数
		unsigned int windowWidth = 0;				// 窗口宽度
		unsigned int windowHeight = 0;				// 窗口高度

		// 完全一致的位图信息头，和bmiInit结合起来使用避免多次重置浪费时间
		BITMAPINFO bmi{};
		bool bmiInit = false;

		// 屏幕缓冲层
		Layer screenBufferLayer;

		// 记录世界中所有画布的字典，实际上大部分时间是被当做vector用的
		std::map<unsigned int, EngineTools::Canvas*> CanvasLayer;

		static RenderSystem* pRenderSystem;		// 指向一个实例的指针

	public:
		// 获取一个实例的指针，单例模式
		// @ret	RenderSystem*	获取到的实例的指针
		static RenderSystem* GetInstance();

		// 更改渲染系统大小
		// @para width	新的宽度
		// @para height	新的高度
		void Resize(unsigned int width, unsigned int height);

		// 获取当前窗口大小（与渲染器大小一致）
		// @ret std::pair<unsigned int, unsigned int>
		//		first	窗口高度
		//		second	窗口宽度
		std::pair<unsigned int, unsigned int> GetWindowSize() const;

		// 在屏幕上绘制图形
		// @para hdc	将要绘制的设备的句柄
		// @para glSet	全局信息保存点，即每个场景中都应该有的GlobalSettings组件的指针
		void Draw(HDC hdc, EngineTools::GlobalSettings* glSet);
		
		// 注册画布
		// @para CanvasUID	画布的UID
		// @para canvas		画布组件的指针
		void RegisterCanvas(unsigned int CanvasUID, EngineTools::Canvas* canvas);

		// 移除指定UID的画布
		// @para CanvasUID	画布的UID
		void RemoveCanvas(unsigned int CanvasUID);

		// 渲染所有画布，并得到一个结果层作为UI的集合体
		// @ret	Layer	UI渲染结果
		Layer RenderCanvasLayer();
	};
}


#endif // SRENDER_RENDERSYSTEM