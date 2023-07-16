#include "Layer.h"

std::mutex colorLock;

void SRender::Layer::ThreadCall(int beginLine, int lineCount, int screenWidth, 
    RGBAColor* resBuffer, std::pair<int, int> resBeginPos, std::pair<int, int> resBufferSize,
    RGBAColor* layerBuffer, std::pair<int, int> layerBeginPos, std::pair<int, int> layerBufferSize,
    RGBAColor* otherBuffer, std::pair<int, int> otherBeginPos, std::pair<int, int> otherBufferSize)
{
    for (int i = beginLine; i < beginLine + lineCount; i++)
    {
        int dispHeight1 = 0;
        int dispHeight2 = 0;
        bool layer1HeightEnable = false;
        bool layer2HeightEnable = false;

        if (layerBeginPos.first <= i && layerBeginPos.first + layerBufferSize.first > i)
        {
            layer1HeightEnable = true;
            dispHeight1 = layerBeginPos.first;
        }

        if (otherBeginPos.first <= i && otherBeginPos.first + otherBufferSize.first > i)
        {
            layer2HeightEnable = true;
            dispHeight2 = otherBeginPos.first;
        }

        for (int j = 0; j < screenWidth; j++)
        {
            int dispWidth1 = 0;
            int dispWidth2 = 0;
            bool layer1WidthEnable = false;
            bool layer2WidthEnable = false;

            if (layerBeginPos.second <= j && layerBeginPos.second + layerBufferSize.second > j)
            {
                layer1WidthEnable = true;
                dispWidth1 = layerBeginPos.second;
            }

            if (otherBeginPos.second <= j && otherBeginPos.second + otherBufferSize.second > j)
            {
                layer2WidthEnable = true;
                dispWidth2 = otherBeginPos.second;
            }

            if (layer1HeightEnable && layer1WidthEnable && layer2HeightEnable && layer2WidthEnable)
            {
                // colorLock.lock();
                (resBuffer)[i * resBufferSize.second + j] =
                    (layerBuffer)[(i - dispHeight1) * layerBufferSize.second + j - dispWidth1] +
                    (otherBuffer)[(i - dispHeight2) * otherBufferSize.second + j - dispWidth2];
                // colorLock.unlock();
            }
            else if (!(layer1HeightEnable && layer1WidthEnable) && layer2HeightEnable && layer2WidthEnable)
            {
                // colorLock.lock();
                (resBuffer)[i * resBufferSize.second + j] =
                    (otherBuffer)[(i - dispHeight2) * otherBufferSize.second + j - dispWidth2];
                // colorLock.unlock();
            }
            else if (layer1HeightEnable && layer1WidthEnable && !(layer2HeightEnable && layer2WidthEnable))
            {
                // colorLock.lock();
                (resBuffer)[i * resBufferSize.second + j] =
                    (layerBuffer)[(i - dispHeight1) * layerBufferSize.second + j - dispWidth1];
                // colorLock.unlock();
            }
            else
            {
                // colorLock.lock();
                (resBuffer)[i * resBufferSize.second + j] = { 0, 0, 0, 0 };
                // colorLock.unlock();
            }

        }
    }
}
