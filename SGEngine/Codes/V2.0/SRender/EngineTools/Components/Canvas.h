/*
*	Created by tianlan.
*	画布，用于承载UI的绘制组件
* 
*	class Canvas;
*/
#ifndef SGENGINE_CANVAS_H
#define SGENGINE_CANVAS_H

#include <map>
#include "../Component.h"
#include "../../Layer.h"
#include "../../RenderSystem.h"
#include "../UIBase.h"

using namespace SRender;

namespace EngineTools 
{
	class Canvas : public Component
	{
		static unsigned int CanvasCount;					// 已经创建的画布数量
		std::pair<unsigned int, unsigned int> canvasSize;	// 画布大小
		std::map<unsigned int, UIBase*> UIs;				// 注册到该画布的UI
		Layer* defaultBackGround = nullptr;					// 默认背景
		Layer* outPut = nullptr;							// 向渲染系统输出的层
		bool needRepaint = true;							// 标记是否需要重绘
		bool isResigered = false;							// 标记是否已向渲染系统注册

		// 重新绘制画布
		void Repaint();

	public:
		unsigned int CanvasID;								// 画布ID

		// 构造函数
		Canvas();

		~Canvas();

		void Start();

		void Update();

		// 当启用的时候，注册到渲染系统
		void OnEnable();

		// 当禁用的时候，取消注册
		void OnDisable();

		// 获取当前画布的输出
		Layer* GetOutput() const;

		// 把目标UI注册到当前画布
		// @para UIID		UI的ID
		// @para UILayer	UI的指针
		void RegisterUIElement(unsigned int UIID, UIBase* UILayer);

		// 移除指定ID的UI
		// @para UIID		UI的ID
		void RemoveUIElement(unsigned int UIID);

		// 把当前画布注册到渲染系统
		void RegisterCanvasToRenderSystem();

		// 从渲染系统移除当前画布
		void RemoveCanvasFromRenderSystem();

		// UI项通知画布有变化，需要重绘
		void InformChange();

	};
}

#endif
