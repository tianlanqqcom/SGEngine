//
// Created by tianlan on 2023/5/12.
//

#include "DebugHelper.h"

namespace EngineTools
{
    void DebugHelper::Log(const char* s, const char* fileName)
    {
        time_t nowTime = time(nullptr);
        char st[64];
        strftime(st, sizeof(st), "%Y-%m-%d %H:%M:%S", localtime(&nowTime));
        if (strcmp("", fileName) == 0)
        {
            std::cout << s << " at " << st << std::endl;
        }
        else
        {
            std::fstream logFile;

            // 以附加模式打开文件，避免覆盖以前的日志
            logFile.open(fileName, std::ios::out | std::ios::app);
            logFile << s << " at " << st << std::endl;
            logFile.close();
        }
    }
    
    void DebugHelper::LogWarning(const char* s, const char* fileName)
    {
        time_t nowTime = time(nullptr);
        char st[64];
        strftime(st, sizeof(st), "%Y-%m-%d %H:%M:%S", localtime(&nowTime));
        if (strcmp("", fileName) == 0)
        {
            std::cout << "\033[33mWarning: " << s << " at " << st << "\033[0m" << std::endl;
        }
        else
        {
            std::fstream logFile;
            logFile.open(fileName, std::ios::out | std::ios::app);
            logFile << "Warning: " << s << " at " << st << std::endl;
            logFile.close();
        }
    }
    
    void DebugHelper::LogError(const char* s, const char* fileName)
    {
        time_t nowTime = time(nullptr);
        char st[64];
        strftime(st, sizeof(st), "%Y-%m-%d %H:%M:%S", localtime(&nowTime));
        if (strcmp("", fileName) == 0)
        {
            std::cout << "\033[31mError: " << s << " at " << st << "\033[0m" << std::endl;
        }
        else
        {
            std::fstream logFile;
            logFile.open(fileName, std::ios::out | std::ios::app);
            logFile << "Error: " << s << " at " << st << std::endl;
            logFile.close();
        }
    }
    
    void DebugHelper::LogError(std::exception& e, const char* fileName)
    {
        if (strcmp("", fileName) == 0)
        {
            std::cout << "\033[31mError: " << e.what() << "\033[0m" << std::endl;
        }
        else
        {
            std::fstream logFile;
            logFile.open(fileName, std::ios::out | std::ios::app);
            logFile << "Error: " << e.what() << std::endl;
            logFile.close();
        }
    }
} // EngineTools4