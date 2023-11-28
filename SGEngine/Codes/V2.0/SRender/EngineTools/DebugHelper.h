//
// Created by tianlan on 2023/5/12.
// Debug���������������־��Ϣ
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
        // �����־��Ϣ
        // @ para T* object ��Ҫ�鿴��Ϣ������ָ�룬��������Ҫӵ��ToString����
        // @ para const std::string& fileName = ""  �����־��λ�ã����Ϊ���ַ��������������̨�����������Ŀ���ļ�
        template<typename T>
        static void Log(T* object, const std::string& fileName = "");

        static void Log(const char* s, const char* fileName = "");

        static void LogWarning(const char* s, const char* fileName = "");

        static void LogError(const char* s, const char* fileName = "");

        static void LogError(std::exception& e, const char* fileName = "");

    };


    // �����־��Ϣ
    // @ para T* object ��Ҫ�鿴��Ϣ������ָ�룬��������Ҫӵ��ToString����
    // @ para const std::string& fileName = ""  �����־��λ�ã����Ϊ���ַ��������������̨�����������Ŀ���ļ�

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

            // �Ը���ģʽ���ļ������⸲����ǰ����־
            logFile.open(fileName, std::ios::out | std::ios::app);
            logFile << object->ToString() << " at " << st << std::endl;
            logFile.close();
        }
    }

} // EngineTools4

#endif //SGENGINE_DEBUGHELPER_H
