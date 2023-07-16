#include "GlobalSettings.h"

std::string EngineTools::GlobalSettings::ToString(int depth) const
{
    std::string ret = "";
    for (int i = 0; i < depth; i++)
    {
        ret += ' ';
    }
    ret += "-GlobalSettings:\n";
    for (int i = 0; i < depth; i++)
    {
        ret += ' ';
    }
    ret += "@EngineVersion: " + EngineVersion + '\n';

    return ret;
}
