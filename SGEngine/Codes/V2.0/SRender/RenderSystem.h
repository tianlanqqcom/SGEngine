/*
*	Created by tianlan.
*	��������Ⱦϵͳ��Ҫ���Ʋ��֣��ռ����������л�����������Ϣ
*	���Ʋ����Ǹ��������ĵ�λ���뾡�����ٻ��ƻ����������ʹ���
* 
*	�����ࣺ
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
	class Canvas;	// ǰ������
}

namespace SRender
{
	class RenderSystem
	{
	private:
		RenderSystem() = default;					// Ĭ�Ϲ��캯��
		unsigned int windowWidth = 0;				// ���ڿ��
		unsigned int windowHeight = 0;				// ���ڸ߶�

		// ��ȫһ�µ�λͼ��Ϣͷ����bmiInit�������ʹ�ñ����������˷�ʱ��
		BITMAPINFO bmi{};
		bool bmiInit = false;

		// ��Ļ�����
		Layer screenBufferLayer;

		// ��¼���������л������ֵ䣬ʵ���ϴ󲿷�ʱ���Ǳ�����vector�õ�
		std::map<unsigned int, EngineTools::Canvas*> CanvasLayer;

		static RenderSystem* pRenderSystem;		// ָ��һ��ʵ����ָ��

	public:
		// ��ȡһ��ʵ����ָ�룬����ģʽ
		// @ret	RenderSystem*	��ȡ����ʵ����ָ��
		static RenderSystem* GetInstance();

		// ������Ⱦϵͳ��С
		// @para width	�µĿ��
		// @para height	�µĸ߶�
		void Resize(unsigned int width, unsigned int height);

		// ��ȡ��ǰ���ڴ�С������Ⱦ����Сһ�£�
		// @ret std::pair<unsigned int, unsigned int>
		//		first	���ڸ߶�
		//		second	���ڿ��
		std::pair<unsigned int, unsigned int> GetWindowSize() const;

		// ����Ļ�ϻ���ͼ��
		// @para hdc	��Ҫ���Ƶ��豸�ľ��
		// @para glSet	ȫ����Ϣ����㣬��ÿ�������ж�Ӧ���е�GlobalSettings�����ָ��
		void Draw(HDC hdc, EngineTools::GlobalSettings* glSet);
		
		// ע�ử��
		// @para CanvasUID	������UID
		// @para canvas		���������ָ��
		void RegisterCanvas(unsigned int CanvasUID, EngineTools::Canvas* canvas);

		// �Ƴ�ָ��UID�Ļ���
		// @para CanvasUID	������UID
		void RemoveCanvas(unsigned int CanvasUID);

		// ��Ⱦ���л��������õ�һ���������ΪUI�ļ�����
		// @ret	Layer	UI��Ⱦ���
		Layer RenderCanvasLayer();
	};
}


#endif // SRENDER_RENDERSYSTEM