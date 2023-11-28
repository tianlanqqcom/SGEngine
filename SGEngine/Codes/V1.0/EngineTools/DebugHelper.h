//
// Created by tianlan on 2023/5/12.
// Debug���������������־��Ϣ
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
        // �����־��Ϣ
        // @ para T* object ��Ҫ�鿴��Ϣ������ָ�룬��������Ҫӵ��ToString����
        // @ para const std::string& fileName = ""  �����־��λ�ã����Ϊ���ַ��������������̨�����������Ŀ���ļ�
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

                // �Ը���ģʽ���ļ������⸲����ǰ����־
                logFile.open(fileName, std::ios::out | std::ios::app);
                logFile << object->ToString() << std::endl;
                logFile.close();
            }
        }

    };

} // EngineTools4

#endif //SGENGINE_DEBUGHELPER_H
