#include "Image.h"

EngineTools::Image::Image()
{
	typeHashCode = typeid(Image).hash_code();
}

void EngineTools::Image::BindImage(const std::string& path, ImageType type)
{
	imgData = ResourceManager::GetInstance()->LoadImageAtPath(path, type);
	if (imgData)
	{
		out = imgData->ScaleTo(height, width);
	}

	// ÌÞ³ýÍ¼Æ¬ÖÐµÄ°×µã
	/*for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (out->GetValue(i, j) == RGBAColor(255, 255, 255, 255))
			{
				out->SetValue(i, j, RGBAColor(255, 255, 255, 0));
			}
		}
	}*/
	hasChange = true;
}

void EngineTools::Image::Start()
{
	tempPosX = posX;
	tempPosY = posY;
	if (imgData && hasChange)
	{
		if (canvas)
		{
			canvas->RegisterUIElement(gameObject->UID, this);
		}
		hasChange = false;
	}
}

void EngineTools::Image::Update()
{
	if (tempPosX != posX || tempPosY != posY)
	{
		hasChange = true;
		tempPosX = posX;
		tempPosY = posY;
	}

	if (canvas && !isRegistered)
	{
		canvas->RegisterUIElement(gameObject->UID, this);
		isRegistered = false;
	}

	if (imgData && hasChange)
	{
		if (canvas)
		{
			canvas->InformChange();
		}

		if (!out)
		{
			// out = new Layer;
			out = new SRender::MathTools::Matrix<RGBAColor>(height, width, { 0,0,0,0 });
		}
		
		hasChange = false;
	}
}

inline void EngineTools::Image::MoveTo(const std::pair<int, int>& position)
{
	posX = position.first;
	posY = position.second;
}

inline void EngineTools::Image::Translate(const std::pair<int, int>& vec)
{
	posX += vec.first;
	posY += vec.second;
}

void EngineTools::Image::ScaleTo(int nHeight, int nWidth)
{
	height = nHeight;
	width = nWidth;
	if (imgData)
	{
		if (out)
		{
			delete out;
		}

		out = imgData->ScaleTo(height, width);
		hasChange = true;
	}
}

void EngineTools::Image::BindCanvas(Canvas* pCanvas)
{
	if (pCanvas)
	{
		canvas = pCanvas;
		canvas->RegisterUIElement(gameObject->UID, this);
	}
}

void EngineTools::Image::BindCanvas(const std::string& path)
{
	auto ret = gameObject->scene->GetChild(path)->GetComponent<Canvas>();
	if (ret)
	{
		canvas = ret;
		canvas->RegisterUIElement(gameObject->UID, this);
	}
}