//
// Created by tianlan on 2023/5/12.
// Debug帮助，用于输出日志信息
//

#ifndef SGENGINE_DEBUGHELPER_H
#define SGENGINE_DEBUGHELPER_H
#include <iostream>
#include <fstream>

namespace EngineTools
{

    class DebugHelper
    {
    public:
        // 输出日志信息
        // @ para T* object 需要查看信息的物体指针，该物体需要拥有ToString方法
        // @ para const std::string& fileName = ""  输出日志的位置，如果为空字符串，输出到控制台，否则输出到目标文件
        template<typename T>
        static void Log(T* object, const std::string& fileName = "")
        {
            if (fileName.empty())
            {
                std::cout << object->ToString() << std::endl;
            }
            else
            {
                std::fstream logFile;

                // 以附加模式打开文件，避免覆盖以前的日志
                logFile.open(fileName, std::ios::out | std::ios::app);
                logFile << object->ToString() << std::endl;
                logFile.close();
            }
        }

    };

} // EngineTools4

#endif //SGENGINE_DEBUGHELPER_H
