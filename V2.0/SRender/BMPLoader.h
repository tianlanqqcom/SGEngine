#ifndef SRENDER_BMPLOADER
#define SRENDER_BMPLOADER
#include <fstream>
#include "MathTools.h"
#include "RGBColor.h"

using namespace SRender;
using namespace SRender::MathTools;

#pragma pack(1)
typedef struct tagBITMAPFILEHEADER2
{
	unsigned short bfType;
	long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	long bfOffBits;
} BITMAPFILEHEADER2;


typedef struct
{
	long infoSize;
	long imWidth;
	long imHeight;
	unsigned short always1;
	unsigned short colorSize;
	long imType;
	long imSize;
	long iw_ignore;
	long ih_ignore;
	long realColorIndexCount;
	long importantColorIndexCount;
} BITMAPINFOHEADER2;
#pragma pack()
//typedef struct tagRGBQUAD
//{
//	unsigned char b;
//	unsigned char g;
//	unsigned char r;
//	unsigned char reserved;
//} RGBQUAD;

class BMPLoader
{
public:
	static Matrix<RGBAColor> ReadBMP(const std::string& filePath)
	{
		std::ifstream bmpFile(filePath, std::ios::binary);
		// bmpFile.open(filePath, std::ios::binary | std::ios::in);

		if (!bmpFile)
		{
			throw "Failed to open";
		}


		BITMAPFILEHEADER2 fHeader = {};
		BITMAPINFOHEADER2 fInfo = {};
		// bmpFile >> fHeader.bfType;

		bmpFile.read((char*)&fHeader, sizeof(BITMAPFILEHEADER2));
		if (fHeader.bfType != 0x4d42)
		{
			bmpFile.close();
			return Matrix<RGBAColor>(0, 0, { 0,0,0,0 });
		}
		/*MathTools::Matrix<RGBAColor> res(50, 50);
		bmpFile >> fHeader.bfSize >> fHeader.bfReserved1 >> fHeader.bfReserved2 >> fHeader.bfOffBits;
		bmpFile >> fInfo.infoSize >> fInfo.imWidth >> fInfo.imHeight >> fInfo.always1 >> fInfo.colorSize >> fInfo.imType
			>> fInfo.imSize >> fInfo.iw_ignore >> fInfo.ih_ignore >> fInfo.realColorIndexCount >> fInfo.importantColorIndexCount;*/
		bmpFile.read((char*)&fInfo, sizeof(BITMAPINFOHEADER2));

		MathTools::Matrix<RGBAColor> res(fInfo.imHeight, fInfo.imWidth);
		int matIndex = 0;
		// bmpFile.seekg(fHeader.bfOffBits);
		char* colorBuff = new char[fInfo.colorSize / 8 * res.GetCol() * res.GetRow()];
		// bmpFile.read((char*)colorBuff, (fInfo.colorSize / 8) * res.GetCol() * res.GetRow());
		bmpFile.read(colorBuff, fHeader.bfSize - 54);
		auto tempPointer = colorBuff;
		if (fHeader.bfSize < (fInfo.colorSize / 8) * res.GetCol() * res.GetRow())
		{
			int b = fHeader.bfSize;
		}
		for (int i = 0; i < (fInfo.colorSize / 8) * res.GetCol() * res.GetRow(); i++)
		{
			if (colorBuff[i] != -1)
			{
				int a = i;
			}
		}

		while (true)
		{
			
			// bmpFile >> b >> g >> r;
			res[matIndex / fInfo.imWidth][matIndex % fInfo.imWidth] = { (unsigned char)*tempPointer,(unsigned char)*(tempPointer + 1),(unsigned char)*(tempPointer + 2),255 };
			tempPointer += (fInfo.colorSize / 8);
			matIndex++;

			if (matIndex >= res.GetCol() * res.GetRow())
			{
				break;
			}
		}
		delete[] colorBuff;
		bmpFile.close();

		// MathTools::Matrix<RGBAColor> realRes(fInfo.imWidth, fInfo.imHeight);
		/*for (int i = 0; i < fInfo.imWidth; i++)
		{
			for (int j = fInfo.imHeight - 1; j >= 0; j--)
			{
				realRes[i][fInfo.imHeight - 1 - j] = res[j][i];
			}
		}*/
		MathTools::Matrix<RGBAColor> realRes(fInfo.imHeight, fInfo.imWidth);
		for (int i = 0; i < fInfo.imHeight; i++)
		{
			int realI = fInfo.imHeight - 1 - i;
			for (int j = 0;j < fInfo.imWidth; j++)
			{
				realRes[realI][j] = res[i][j];
			}
		}

		return realRes;
	}
};
#endif // SRENDER_BMPLOADER



