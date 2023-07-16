#ifndef SRENDER_LAYER
#define SRENDER_LAYER

#include <thread>
#include <vector>
#include <mutex>
#include "MathTools.h"
#include "RGBColor.h"

namespace SRender
{
	enum class LayerType
	{
		UILayer,
		GameWorld
	};

	class Layer
	{
		MathTools::Matrix<RGBAColor> layerBuffer;
		LayerType layerType = LayerType::UILayer;
		int layerWidth = 0;
		int layerHeight = 0;

		std::pair<int, int> layerBeginPos = { 0,0 }; // Upper-Left Position of the layer, beginPos = {height, width};

        static bool PointIsInLayer(std::pair<int, int>& point, std::pair<int, int>& beginPos, int lWidth, int lHeight)
        {
            if (beginPos.first <= point.first && beginPos.first + lWidth > point.first &&
                beginPos.second <= point.second && beginPos.second + lHeight > point.second)
            {
                return true;
            }
            return false;
        }


	public:
		Layer() = default;
		Layer(int width, int height)
		{
			layerHeight = height;
			layerWidth = width;
			layerBuffer.ReSize(height, width);
		}

        Layer(int width, int height, RGBAColor newColor)
        {
            layerHeight = height;
            layerWidth = width;
            layerBuffer.ReSize(height, width, newColor);
        }

        Layer(int width, int height, int newColor)
        {
            layerHeight = height;
            layerWidth = width;
            layerBuffer.ReSize(height, width, newColor);
        }

		Layer(int width, int height, std::pair<int, int>& beginPos)
		{
			layerHeight = height;
			layerWidth = width;
			layerBeginPos = beginPos;
			layerBuffer.ReSize(height, width);
		}

		void ReSize(int width, int height)
		{
			layerHeight = height;
			layerWidth = width;
			layerBuffer.ReSize(height, width);
		}

        void ReSize(int width, int height, RGBAColor newColor)
        {
            layerHeight = height;
            layerWidth = width;
            layerBuffer.ReSize(height, width, newColor);
        }

        void ReSize(int width, int height, int newColor)
        {
            layerHeight = height;
            layerWidth = width;
            layerBuffer.ReSize(height, width, newColor);
        }

        MathTools::Matrix<RGBAColor> GetLayerBuffer() const
        {
            return layerBuffer;
        }

        RGBAColor* GetLayerBufferPtr() const
        {
            return layerBuffer.GetValuePtr();
        }

		std::pair<int,int> GetLayerSize() const
		{
			return { layerHeight,layerWidth };
		}

		LayerType GetLayerType() const
		{
			return layerType;
		}

		std::pair<int, int> GetLayerBeginPos() const
		{
			return layerBeginPos;
		}

		void SetLayerType(LayerType type)
		{
			layerType = type;
		}

		void SetLayerBeginPos(std::pair<int, int> pos)
		{
			layerBeginPos = pos;
		}

		void SetLayerBuffer(const MathTools::Matrix<RGBAColor>& buffer)
		{
			layerHeight = buffer.GetRow();
			layerWidth = buffer.GetCol();
			layerBuffer = buffer;
		}

		// 与另一个层相加，返回一个新的层，新层尽最大可能保留两个层的所有内容并使内存最小
		// @ para	Layer&	other	另一个层
		// @ ret	Layer			相加得到的结果
        Layer Add(Layer& other, int screenWidth, int screenHeight) const
        {
            // 结果层
            Layer res(screenWidth, screenHeight, 0);
            unsigned int cpuSize = std::thread::hardware_concurrency();

            unsigned int threadCalSize = screenHeight / cpuSize;
            std::vector<std::thread> threadings;

            auto resPtr = res.GetLayerBufferPtr();
            auto layerPtr = this->GetLayerBufferPtr();
            auto otherPtr = other.GetLayerBufferPtr();

            auto resBeginPos = res.GetLayerBeginPos();
            auto layerBeginPos = this->GetLayerBeginPos();
            auto otherBeginPos = other.GetLayerBeginPos();

            auto resSize = res.GetLayerSize();
            auto layerSize = this->GetLayerSize();
            auto otherSize = other.GetLayerSize();

            unsigned int nowLine = 0;
            unsigned int cpuSizeSub1 = cpuSize - 1;

            for (int i = 0; i < cpuSizeSub1; i++)
            {
                threadings.emplace_back(ThreadCall, nowLine, threadCalSize, screenWidth, 
                    resPtr, resBeginPos, resSize,
                    layerPtr, layerBeginPos, layerSize,
                    otherPtr, otherBeginPos, otherSize);
                nowLine += threadCalSize;
            }
            //threadings.emplace_back(ThreadCall, 7 * threadCalSize, screenHeight - 7 * threadCalSize, screenWidth,
            //    res.GetLayerBufferPtr(), (res.GetLayerBeginPos()), (res.GetLayerSize()),
            //    this->GetLayerBufferPtr(), (this->GetLayerBeginPos()), (this->GetLayerSize()),
            //    other.GetLayerBufferPtr(), (other.GetLayerBeginPos()), (other.GetLayerSize()));

            for (int i = nowLine; i < screenHeight; i++)
            {
                int dispHeight1 = 0;
                int dispHeight2 = 0;
                bool layer1HeightEnable = false;
                bool layer2HeightEnable = false;

                if (layerBeginPos.first <= i && layerBeginPos.first + layerHeight > i)
                {
                    layer1HeightEnable = true;
                    dispHeight1 = layerBeginPos.first;
                }

                if (other.layerBeginPos.first <= i && other.layerBeginPos.first + other.layerHeight > i)
                {
                    layer2HeightEnable = true;
                    dispHeight2 = other.layerBeginPos.first;
                }

                for (int j = 0; j < screenWidth; j++)
                {
                    int dispWidth1 = 0;
                    int dispWidth2 = 0;
                    bool layer1WidthEnable = false;
                    bool layer2WidthEnable = false;

                    if (layerBeginPos.second <= j && layerBeginPos.second + layerWidth > j)
                    {
                        layer1WidthEnable = true;
                        dispWidth1 = layerBeginPos.second;
                    }

                    if (other.layerBeginPos.second <= j && other.layerBeginPos.second + other.layerWidth > j)
                    {
                        layer2WidthEnable = true;
                        dispWidth2 = other.layerBeginPos.second;
                    }

                    if (layer1HeightEnable && layer1WidthEnable && layer2HeightEnable && layer2WidthEnable)
                    {
                        // res.layerBuffer[i][j] =
                        //     other.layerBuffer[i - dispHeight2][j - dispWidth2] +
                        //     layerBuffer[i - dispHeight1][j - dispWidth1];
                        res.GetLayerBufferPtr()[i * screenWidth + j] = 
                            layerBuffer[i - dispHeight1][j - dispWidth1] +
                            other.layerBuffer[i - dispHeight2][j - dispWidth2];
                    }
                    else if (!(layer1HeightEnable && layer1WidthEnable) && layer2HeightEnable && layer2WidthEnable)
                    {
                        // res.layerBuffer[i][j] =
                        //     other.layerBuffer[i - dispHeight2][j - dispWidth2];
                        res.GetLayerBufferPtr()[i * screenWidth + j] =
                            other.layerBuffer[i - dispHeight2][j - dispWidth2];
                    }
                    else if (layer1HeightEnable && layer1WidthEnable && !(layer2HeightEnable && layer2WidthEnable))
                    {
                        // res.layerBuffer[i][j] =
                        //     layerBuffer[i - dispHeight1][j - dispWidth1];
                        res.GetLayerBufferPtr()[i * screenWidth + j] = layerBuffer[i - dispHeight1][j - dispWidth1];
                    }
                    else
                    {
                        // res.layerBuffer[i][j] = { 0, 0, 0, 0 };
                        res.GetLayerBufferPtr()[i * screenWidth + j] = { 0,0,0,0 };
                    }

                }
            }

            for (auto& threading : threadings)
            {
                threading.join();
            }

            /*for (int i = 0; i < screenHeight; i++)
            {
                for (int j = 0; j < screenWidth; j++)
                {
                    std::pair<int, int> nowPoint = { i,j };
                    bool thisLayerEnable = PointIsInLayer(nowPoint, layerBeginPos, layerWidth, layerHeight);
                    bool otherLayerEnable = PointIsInLayer(nowPoint, other.layerBeginPos, other.layerWidth, other.layerHeight);
                    if (thisLayerEnable && otherLayerEnable)
                    {
                        res.GetLayerBufferPtr()[i * screenWidth + j] =
							layerBuffer[i - layerBeginPos.first][j - layerBeginPos.second] +
							other.layerBuffer[i - otherBeginPos.first][j - otherBeginPos.second];
                    }
                    else if (thisLayerEnable)
                    {
                        res.GetLayerBufferPtr()[i * screenWidth + j] =
                            layerBuffer[i - layerBeginPos.first][j - layerBeginPos.second];
                    }
                    else if (otherLayerEnable)
                    {
                        res.GetLayerBufferPtr()[i * screenWidth + j] =
                            other.layerBuffer[i - otherBeginPos.first][j - otherBeginPos.second];
                    }
                    else
                    {
                        res.GetLayerBufferPtr()[i * screenWidth + j] = RGBAColor(0, 0, 0, 0);
                    }
                }
            }*/

            return res;

            //// 用于层起始点的偏移值
            //// 相对于某个层的偏移
            //int dispX = 0;
            //int dispY = 0;

            //// 用于层结束点的偏移值
            //int edispX = 0;
            //int edispY = 0;

            //// 标记层偏移是相对于哪一个层的
            //bool bDispX_F1 = false;
            //bool bDispX_F2 = false;
            //bool bDispY_F1 = false;
            //bool bDispY_F2 = false;

            //bool beDispX_F1 = false;
            //bool beDispX_F2 = false;
            //bool beDispY_F1 = false;
            //bool beDispY_F2 = false;

            //if (layerBeginPos == other.layerBeginPos)
            //{
            //	width = layerWidth >= other.layerWidth ? layerWidth : other.layerWidth;
            //	height = layerHeight >= other.layerHeight ? layerHeight : other.layerHeight;
            //	res.ReSize(width, height);
            //}
            //else
            //{
            //	int nPosx = 0;
            //	int nPosy = 0;

            //	if (layerBeginPos.first < other.layerBeginPos.first)
            //	{
            //		nPosx = layerBeginPos.first;
            //		dispX = other.layerBeginPos.first - other.layerBeginPos.first;
            //		bDispX_F2 = true;
            //	}
            //	else if (other.layerBeginPos.first > layerBeginPos.first)
            //	{
            //		nPosx = other.layerBeginPos.first;
            //		dispX = layerBeginPos.first - other.layerBeginPos.first;
            //		bDispX_F1 = true;
            //	}
            //	else
            //	{
            //		nPosx = nPosx = layerBeginPos.first;
            //	}

            //	if (layerBeginPos.second < other.layerBeginPos.second)
            //	{
            //		nPosy = layerBeginPos.second;
            //		dispY = other.layerBeginPos.second - other.layerBeginPos.second;
            //		bDispY_F2 = true;
            //	}
            //	else if (other.layerBeginPos.second > layerBeginPos.second)
            //	{
            //		nPosy = other.layerBeginPos.second;
            //		dispY = layerBeginPos.second - other.layerBeginPos.second;
            //		bDispY_F1 = true;
            //	}
            //	else
            //	{
            //		nPosy = nPosx = layerBeginPos.second;
            //	}

            //	// 表示两个层右下角的点
            //	int endPointX1 = layerBeginPos.first + layerWidth;
            //	int endPointX2 = other.layerBeginPos.first + other.layerWidth;
            //	int endPointY1 = layerBeginPos.second + layerHeight;
            //	int endPointY2 = other.layerBeginPos.second + other.layerHeight;

            //	int finalEndPointX = 0;
            //	int finalEndPointY = 0;

            //	if (endPointX1 < endPointX2)
            //	{

            //	}

            //	
            //}

        }

        Layer Add(std::vector<Layer*> layers, int screenWidth, int screenHeight) const
        {
            Layer res = *this;
            for (auto& layer : layers)
            {
                res = res.Add(*layer, screenWidth, screenHeight);
            }
            return res;
        }

        Layer& AddWithMe(Layer* other)
        {
            auto beginPos = other->GetLayerBeginPos();
            int beginRow = beginPos.first < 0 ? 0 : beginPos.first;
            int beginCol = beginPos.second < 0 ? 0 : beginPos.second;

            if (beginRow >= this->layerHeight || beginCol >= this->layerWidth)
            {
                return *this;
            }

            int dispHeight = 0;
            int maxDispHeight = other->layerHeight;

            int dispWidth = 0;
            int maxDispWidth = other->layerWidth;

            for (int i = beginRow; i < layerHeight && dispHeight < maxDispHeight; i++, dispHeight++)
            {
                for (int j = beginCol; j < layerWidth && dispWidth < maxDispWidth; j++, dispWidth++)
                {
                    this->layerBuffer[i][j] = other->layerBuffer[dispHeight][dispWidth];
                }
            }

            return *this;
        }



        static void ThreadCall(int beginLine, int lineCount, int screenWidth,
            RGBAColor* resBuffer, std::pair<int, int> resBeginPos, std::pair<int, int> resBufferSize,
            RGBAColor* layerBuffer, std::pair<int, int> layerBeginPos, std::pair<int, int> layerBufferSize,
            RGBAColor* otherBuffer, std::pair<int, int> otherBeginPos, std::pair<int, int> otherBufferSize
        );
	};

}

#endif // !SRENDER_LAYER



