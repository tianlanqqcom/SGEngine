//
// Created by tianlan on 2023/5/12.
// Debug帮助，用于输出日志信息
//

#ifndef SGENGINE_DEBUGHELPER_H
#define SGENGINE_DEBUGHELPER_H

// Disable VS Errors
#pragma warning(disable:4996)

#include <iostream>
#include <fstream>
#include <ctime>

namespace EngineTools
{

    class DebugHelper
    {
    public:
        // 输出日志信息
        // @ para T* object 需要查看信息的物体指针，该物体需要拥有ToString方法
        // @ para const std::string& fileName = ""  输出日志的位置，如果为空字符串，输出到控制台，否则输出到目标文件
        template<typename T>
        static void Log(T* object, const std::string& fileName = "");

        static void Log(const char* s, const char* fileName = "");

        static void LogWarning(const char* s, const char* fileName = "");

        static void LogError(const char* s, const char* fileName = "");

        static void LogError(std::exception& e, const char* fileName = "");

    };


    // 输出日志信息
    // @ para T* object 需要查看信息的物体指针，该物体需要拥有ToString方法
    // @ para const std::string& fileName = ""  输出日志的位置，如果为空字符串，输出到控制台，否则输出到目标文件

    template<typename T>
    inline void DebugHelper::Log(T* object, const std::string& fileName)
    {
        time_t nowTime = time(nullptr);
        char st[64];
        strftime(st, sizeof(st), "%Y-%m-%d %H:%M:%S", localtime(&nowTime));
        if (fileName.empty())
        {
            std::cout << object->ToString() << " at " << st << std::endl;
        }
        else
        {
            std::fstream logFile;

            // 以附加模式打开文件，避免覆盖以前的日志
            logFile.open(fileName, std::ios::out | std::ios::app);
            logFile << object->ToString() << " at " << st << std::endl;
            logFile.close();
        }
    }

} // EngineTools4

#endif //SGENGINE_DEBUGHELPER_H
