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
	// 重设窗口大小及缓冲层大小
	windowHeight	= height;
	windowWidth		= width;
	screenBufferLayer.ReSize(height, width);

	// 修改bmi信息
	bmi.bmiHeader.biWidth = windowWidth;
	bmi.bmiHeader.biHeight = -(int)windowHeight; // top-down
}

std::pair<unsigned int, unsigned int> SRender::RenderSystem::GetWindowSize() const
{
	return { windowHeight, windowWidth };
}

void SRender::RenderSystem::Draw(HDC hdc, EngineTools::GlobalSettings* glSet)
{
	// 创建一个默认基底
	Layer base(windowWidth, windowHeight, { 0,0,0,0 });

	// 如果有物体信息需要绘制，使用Phong模型绘制物体（Shader可改）
	// 虽然Shader可改，但是在一次绘制里，Shader只能是一样的
	if (glSet)
	{
		PhongShader shader;
		base = shader.Draw(windowWidth, windowHeight, glSet);
	}

	// 渲染UI
	auto UILayer = RenderCanvasLayer();

	// 把UI覆盖到世界物体上，得到完整的屏幕结果
	screenBufferLayer = base.Add(UILayer, windowWidth, windowHeight);

	// 下面把屏幕缓冲输出到屏幕上

	// 如果bmi未初始化，初始化bmi
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

	// 创建一个位图
	COLORREF* pPixels = nullptr;
	HBITMAP hBitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pPixels, NULL, 0);

	// 如果位图创建失败，直接返回
	if (!hBitmap)
	{
		return;
	}

	// 获取层的缓冲数据矩阵
	auto screenBuffer2 = screenBufferLayer.GetLayerBuffer();

	// 丢弃缓冲数据中的透明度信息，把RGB数据赋值给位图
	for (int i = 0; i < windowHeight; i++)
	{
		int pDisp = i * windowWidth;
		for (int j = 0; j < windowWidth; j++)
		{
			auto& temp = screenBuffer2[i][j];
			pPixels[pDisp + j] = RGB(temp.red, temp.green, temp.blue);
		}
	}

	// 把位图绘制到窗口上
	HDC hdcMem = CreateCompatibleDC(hdc);
	SelectObject(hdcMem, hBitmap);
	BitBlt(hdc, 0, 0, windowWidth, windowHeight, hdcMem, 0, 0, SRCCOPY);

	// 清理
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
	// 创建结果层
	Layer res(windowWidth, windowHeight, { 0,0,0,0 });

	// 对于已经注册的所有画布，如果为活跃状态，则绘制其对应的UI层
	for (std::pair<const unsigned int, EngineTools::Canvas*>& canvas : CanvasLayer)
	{
		if (!canvas.second->isActive)
		{
			continue;
		}

		res = res.Add(*(canvas.second->GetOutput()), windowWidth, windowHeight);
	}

	// 清理UI层所有黑色且不透明的像素，很明显，UI里不能有黑点，
	// 如果你真的希望UI里有黑色，可以用RGBA(1,1,1,255)这种颜色
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
