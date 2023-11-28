#include "ImageAsset.h"

unsigned int EngineTools::ImageAsset::ImageCount = 0;

EngineTools::ImageAsset::ImageAsset()
{
	ImageID = ImageCount++;
}

EngineTools::ImageAsset::ImageAsset(const std::string& path, ImageType type)
{
	ImageID = ImageCount++;
	Type = type;

	std::vector<unsigned char> pixelData; //the raw pixels
	unsigned int width = 0, height = 0;
	unsigned int errCode = 0;
	unsigned int* dataPtr = nullptr;
	int err = 0;

	// 对不同的图片文件类型，调用不同的API
	switch (type)
	{
	case EngineTools::ImageType::BMP:
		ImgData = BMPLoader::ReadBMP(path);
		break;
	case EngineTools::ImageType::PNG:
		errCode = lodepng::decode(pixelData, width, height, path);
		if (errCode)
		{
			// 如果出错，输出错误信息
			DebugHelper::LogError(lodepng_error_text(errCode), "ImageAssetInfo.log");
		}
		else
		{
			ImgData.ReSize(height, width);
			dataPtr = new unsigned int[width * height];
			err = memcpy_s(dataPtr, static_cast<unsigned long long>(width) * height * sizeof(int), pixelData.data(), pixelData.size());
			if (err)
			{
				DebugHelper::LogError("Error executing memcpy_s.\n", "ImageAssetInfo.log");
				ImgData.ReSize(0, 0);
			}
			else
			{
				ImgData.SetValuePtr((SRender::RGBAColor*)dataPtr);
			}

		}
		break;
	case EngineTools::ImageType::JPG:
		break;
	default:
		break;
	}

	ImageHeight = ImgData.GetRow();
	ImageWidth = ImgData.GetCol();
}

// 缩放图片到新的大小（使用双线性插值法）
// @para height	新的高度
// @para width	新的宽度

MathTools::Matrix<RGBAColor>* EngineTools::ImageAsset::ScaleTo(int height, int width)
{
	MathTools::Matrix<RGBAColor>* resImg = new MathTools::Matrix<RGBAColor>(height, width);
	LinarInter(resImg->GetValuePtr(),
		width * 1.0 / ImageWidth,
		height * 1.0 / ImageHeight,
		height,
		width,
		ImgData.GetRow(),
		ImgData.GetCol()
	);
	return resImg;
}

// 双线性插值法
// @para resImgData		输出图片像素数据组
// @para scaleX			图片在X方向上的缩放比例
// @para scaleY			图片在Y方向上的缩放比例
// @para height			新的高度
// @para width			新的宽度
// @para sourceHeight	原始高度
// @para sourceWidth	原始宽度
// @in	 原始数据直接从当前图片资产中获取

void EngineTools::ImageAsset::LinarInter(RGBAColor* resImgData, double scaleX, double scaleY, int height, int width, int sourceHeight, int sourceWidth)
{
	int maxHeightIndex = height - 1;
	int maxWidthIndex = width - 1;
	for (int i = 0; i < height; i++)
	{
		int nowBeginIndex = i * width;
		double recordDisp = 0.0;
		int nowImgSourceIndex = 0;
		for (int j = 0; j < width; j++)
		{
			// 如果是最后一行
			if (i == maxHeightIndex)
			{
				if (j == 0 || j == maxWidthIndex)
				{
					if (j == 0)
					{
						resImgData[nowBeginIndex] = ImgData[sourceHeight - 1][0];
					}
					else
					{
						resImgData[nowBeginIndex + j] = ImgData[sourceHeight - 1][sourceWidth];
					}

				}
				else
				{
					recordDisp += scaleX;
					if (recordDisp >= 1.0)
					{
						nowImgSourceIndex += (int)recordDisp;
						recordDisp -= (int)recordDisp;
					}
					if (nowImgSourceIndex > sourceWidth - 2)
					{
						nowImgSourceIndex = sourceWidth - 2;
					}
					resImgData[nowBeginIndex + j] = RGBAColor::LinarAdjust(ImgData[sourceHeight - 1][nowImgSourceIndex], ImgData[sourceHeight - 1][nowImgSourceIndex + 1], recordDisp);
				}
			}
			// 如果不是最后一行
			else
			{
				// 如果是当前行的起始点
				if (j == 0)
				{
					// 如果是第一行的起点
					if (i == 0)
					{
						resImgData[0] = ImgData[0][0];
					}
					// 否则计算ky
					else
					{
						double ky = i / scaleY;
						int sourceY = (int)ky;
						if (sourceY > sourceHeight - 2)
						{
							sourceY = sourceHeight - 2;
						}
						ky -= (int)ky;
						resImgData[nowBeginIndex] = RGBAColor::LinarAdjust(ImgData[sourceY][0], ImgData[sourceY + 1][0], ky);
					}
				}
				else if (j == maxWidthIndex)
				{
					if (i == 0)
					{
						resImgData[j] = ImgData[0][sourceWidth - 1];
					}
					else
					{
						double ky = i / scaleY;
						int sourceY = (int)ky;
						if (sourceY > sourceHeight - 2)
						{
							sourceY = sourceHeight - 2;
						}
						ky -= (int)ky;
						resImgData[nowBeginIndex + j] = RGBAColor::LinarAdjust(ImgData[sourceY][sourceWidth - 1],
							ImgData[sourceY + 1][sourceWidth - 1], ky);

					}
				}
				// 如果什么都不是
				else
				{
					double kx = j / scaleX;
					double ky = i / scaleY;
					int sourceX = (int)kx;
					int sourceY = (int)ky;

					if (sourceX > sourceWidth - 2)
					{
						sourceX = sourceWidth - 2;
					}
					if (sourceY > sourceHeight - 2)
					{
						sourceY = sourceHeight - 2;
					}

					kx -= (int)kx;
					ky -= (int)ky;

					resImgData[nowBeginIndex + j] = RGBAColor::LinarAdjust(
						ImgData[sourceY][sourceX],
						ImgData[sourceY + 1][sourceX],
						ImgData[sourceY][sourceX + 1],
						ImgData[sourceY + 1][sourceX + 1],
						kx, ky
					);
				}
			}
		}
	}
}
