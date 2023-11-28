#include "Canvas.h"

unsigned int EngineTools::Canvas::CanvasCount = 0;

inline void EngineTools::Canvas::Repaint()
{
	if (!outPut)
	{
		outPut = new Layer();
	}

	Layer tempLayer = *defaultBackGround;
	// outPut->SetLayerBuffer(defaultBackGround->GetLayerBuffer());
	std::vector<Layer*> layers;
	for (auto& UILayer : UIs)
	{
		{
			// layers.emplace_back(UILayer.second);
			// tempLayer = tempLayer.Add(*(UILayer.second), canvasSize.second, canvasSize.first);
			// outPut->AddWithMe(UILayer.second);
			auto layerBufferPtr = tempLayer.GetLayerBufferPtr();
			
			int UI_i = 0;
			int UI_j = 0;
			for (int i = UILayer.second->posY; i < UILayer.second->posY + UILayer.second->height; i++, UI_i++)
			{
				if (i >= canvasSize.first || i < 0)
				{
					continue;
				}

				UI_j = 0;
				int dispX = i * canvasSize.second;
				for (int j = UILayer.second->posX; j < UILayer.second->posX + UILayer.second->width; j++, UI_j++)
				{
					if (j >= canvasSize.second || j < 0)
					{
						continue;
					}
					layerBufferPtr[dispX + j] = layerBufferPtr[dispX + j] + UILayer.second->out->GetValue(UI_i, UI_j);
				}
			}
		}
	}
	// outPut->SetLayerBuffer(tempLayer.Add(layers, canvasSize.second, canvasSize.first).GetLayerBuffer());
	outPut->SetLayerBuffer(tempLayer.GetLayerBuffer());
	// outPut = tempLayer;
	// RegisterCanvasToRenderSystem();
}

EngineTools::Canvas::Canvas()
{
	typeHashCode = typeid(Canvas).hash_code();
	CanvasID = CanvasCount++;
	canvasSize = SRender::RenderSystem::GetInstance()->GetWindowSize();
	defaultBackGround = new Layer();
	MathTools::Matrix<RGBAColor> defbg(canvasSize.second, canvasSize.first, { 0,0,0,0 });
	defaultBackGround->SetLayerBuffer(defbg);

}

EngineTools::Canvas::~Canvas()
{
	if (isResigered)
	{
		RemoveCanvasFromRenderSystem();
	}
}

void EngineTools::Canvas::Start()
{
	if (!isResigered)
	{
		RegisterCanvasToRenderSystem();
	}
}

void EngineTools::Canvas::Update()
{
	if (!isResigered)
	{
		RegisterCanvasToRenderSystem();
	}

	auto nowSize = SRender::RenderSystem::GetInstance()->GetWindowSize();
	if (nowSize != canvasSize)
	{
		canvasSize = nowSize;
		defaultBackGround->ReSize(canvasSize.second, canvasSize.first, 0);
		outPut = new Layer(canvasSize.second, canvasSize.first);
		needRepaint = true;
	}

	if (!outPut)
	{
		outPut = new Layer(canvasSize.second, canvasSize.first);
	}

	if (needRepaint)
	{
		Repaint();
		needRepaint = false;
	}
}

void EngineTools::Canvas::OnEnable()
{
	if (!isResigered)
	{
		RegisterCanvasToRenderSystem();
	}
}

void EngineTools::Canvas::OnDisable()
{
	if (isResigered)
	{
		RemoveCanvasFromRenderSystem();
	}
}

Layer* EngineTools::Canvas::GetOutput() const
{
	return outPut;
}

void EngineTools::Canvas::RegisterUIElement(unsigned int UIID, UIBase* UILayer)
{
	UIs[UIID] = UILayer;
	needRepaint = true;
}

void EngineTools::Canvas::RemoveUIElement(unsigned int UIID)
{
	UIs.erase(UIID);
	needRepaint = true;
}

void EngineTools::Canvas::RegisterCanvasToRenderSystem()
{
	RenderSystem::GetInstance()->RegisterCanvas(CanvasID, this);
	isResigered = true;
}

void EngineTools::Canvas::RemoveCanvasFromRenderSystem()
{
	RenderSystem::GetInstance()->RemoveCanvas(CanvasID);
	isResigered = false;
}

void EngineTools::Canvas::InformChange()
{
	needRepaint = true;
}
