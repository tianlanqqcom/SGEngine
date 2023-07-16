//
// Created by tianlan on 2023/4/21.
// 字符串分割工具，后续可能会有用，粘贴自CSDN
//

#ifndef SGENGINE_STRINGTOOLS_H
#define SGENGINE_STRINGTOOLS_H

#include <string>
#include <vector>
#include <sstream>

namespace EngineTools
{
    // 使用字符分割
    void Stringsplit(const std::string &str, char split, std::vector<std::string> &res);

    // 使用字符串分割
    void Stringsplit(const std::string &str, const std::string &splits, std::vector<std::string> &res);

    // 将string类型变量转换为常用的数值类型
    template <typename T>
    T stringToNum(const std::string& str)
    {
        std::istringstream iss(str);
        T num{};
        iss >> num;
        return num;
    }
}
#endif //SGENGINE_STRINGTOOLS_H
