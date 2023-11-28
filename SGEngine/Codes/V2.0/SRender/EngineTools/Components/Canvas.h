/*
*	Created by tianlan.
*	���������ڳ���UI�Ļ������
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
		static unsigned int CanvasCount;					// �Ѿ������Ļ�������
		std::pair<unsigned int, unsigned int> canvasSize;	// ������С
		std::map<unsigned int, UIBase*> UIs;				// ע�ᵽ�û�����UI
		Layer* defaultBackGround = nullptr;					// Ĭ�ϱ���
		Layer* outPut = nullptr;							// ����Ⱦϵͳ����Ĳ�
		bool needRepaint = true;							// ����Ƿ���Ҫ�ػ�
		bool isResigered = false;							// ����Ƿ�������Ⱦϵͳע��

		// ���»��ƻ���
		void Repaint();

	public:
		unsigned int CanvasID;								// ����ID

		// ���캯��
		Canvas();

		~Canvas();

		void Start();

		void Update();

		// �����õ�ʱ��ע�ᵽ��Ⱦϵͳ
		void OnEnable();

		// �����õ�ʱ��ȡ��ע��
		void OnDisable();

		// ��ȡ��ǰ���������
		Layer* GetOutput() const;

		// ��Ŀ��UIע�ᵽ��ǰ����
		// @para UIID		UI��ID
		// @para UILayer	UI��ָ��
		void RegisterUIElement(unsigned int UIID, UIBase* UILayer);

		// �Ƴ�ָ��ID��UI
		// @para UIID		UI��ID
		void RemoveUIElement(unsigned int UIID);

		// �ѵ�ǰ����ע�ᵽ��Ⱦϵͳ
		void RegisterCanvasToRenderSystem();

		// ����Ⱦϵͳ�Ƴ���ǰ����
		void RemoveCanvasFromRenderSystem();

		// UI��֪ͨ�����б仯����Ҫ�ػ�
		void InformChange();

	};
}

#endif
