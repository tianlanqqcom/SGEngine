#include "RenderSystem.h"
#include "EngineTools/Components/Canvas.h"
#include "EngineTools/ResourceManager.h"
#include "PhongShader.h"
using namespace SRender;

RenderSystem* RenderSystem::pRenderSystem = nullptr;

RenderSystem* RenderSystem::GetInstance()
{
	if (!pRenderSystem)
	{
		pRenderSystem = new RenderSystem;
	}
	return pRenderSystem;
}

void SRender::RenderSystem::Resize(unsigned int width, unsigned int height)
{
	// ���贰�ڴ�С��������С
	windowHeight	= height;
	windowWidth		= width;
	screenBufferLayer.ReSize(height, width);

	// �޸�bmi��Ϣ
	bmi.bmiHeader.biWidth = windowWidth;
	bmi.bmiHeader.biHeight = -(int)windowHeight; // top-down
}

std::pair<unsigned int, unsigned int> SRender::RenderSystem::GetWindowSize() const
{
	return { windowHeight, windowWidth };
}

void SRender::RenderSystem::Draw(HDC hdc, EngineTools::GlobalSettings* glSet)
{
	// ����һ��Ĭ�ϻ���
	Layer base(windowWidth, windowHeight, { 0,0,0,0 });

	// �����������Ϣ��Ҫ���ƣ�ʹ��Phongģ�ͻ������壨Shader�ɸģ�
	// ��ȻShader�ɸģ�������һ�λ����Shaderֻ����һ����
	if (glSet)
	{
		PhongShader shader;
		base = shader.Draw(windowWidth, windowHeight, glSet);
	}

	// ��ȾUI
	auto UILayer = RenderCanvasLayer();

	// ��UI���ǵ����������ϣ��õ���������Ļ���
	screenBufferLayer = base.Add(UILayer, windowWidth, windowHeight);

	// �������Ļ�����������Ļ��

	// ���bmiδ��ʼ������ʼ��bmi
	if (!bmiInit)
	{
		bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmi.bmiHeader.biWidth = windowWidth;
		bmi.bmiHeader.biHeight = -(int)windowHeight; // top-down
		bmi.bmiHeader.biPlanes = 1;
		bmi.bmiHeader.biBitCount = 32; // each pixel is 4 bytes (RGBA)
		bmi.bmiHeader.biCompression = BI_RGB;
		bmiInit = true;
	}

	// ����һ��λͼ
	COLORREF* pPixels = nullptr;
	HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);

	// ���λͼ����ʧ�ܣ�ֱ�ӷ���
	if (!hBitmap)
	{
		return;
	}

	// ��ȡ��Ļ������ݾ���
	auto screenBuffer2 = screenBufferLayer.GetLayerBuffer();

	// �������������е�͸������Ϣ����RGB���ݸ�ֵ��λͼ
	for (int i = 0; i < windowHeight; i++)
	{
		int pDisp = i * windowWidth;
		for (int j = 0; j < windowWidth; j++)
		{
			auto& temp = screenBuffer2[i][j];
			pPixels[pDisp + j] = RGB(temp.red, temp.green, temp.blue);
		}
	}

	// ��λͼ���Ƶ�������
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);
	BitBlt(hdc, 0, 0, windowWidth, windowHeight, hdcMem, 0, 0, SRCCOPY);

	// ����
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
}

void SRender::RenderSystem::RegisterCanvas(unsigned int CanvasUID, EngineTools::Canvas* canvas)
{
	CanvasLayer[CanvasUID] = canvas;
}

void SRender::RenderSystem::RemoveCanvas(unsigned int CanvasUID)
{
	CanvasLayer.erase(CanvasUID);
}

Layer SRender::RenderSystem::RenderCanvasLayer()
{
	// ���������
	Layer res(windowWidth, windowHeight, { 0,0,0,0 });

	// �����Ѿ�ע������л��������Ϊ��Ծ״̬����������Ӧ��UI��
	for (std::pair<const unsigned int, EngineTools::Canvas*>& canvas : CanvasLayer)
	{
		if (!canvas.second->isActive)
		{
			continue;
		}

		res = res.Add(*(canvas.second->GetOutput()), windowWidth, windowHeight);
	}

	// ����UI�����к�ɫ�Ҳ�͸�������أ������ԣ�UI�ﲻ���кڵ㣬
	// ��������ϣ��UI���к�ɫ��������RGBA(1,1,1,255)������ɫ
	auto resLayerBufferPtr = res.GetLayerBufferPtr();
	RGBAColor black(0, 0, 0, 255);

	for (int i = 0; i < windowHeight; i++)
	{
		int dispY = i * windowWidth;
		for (int j = 0; j < windowWidth; j++)
		{
			if (resLayerBufferPtr[dispY + j] == black)
			{
				resLayerBufferPtr[dispY + j].alpha = 0;
			}
		}
	}
	return res;
}
