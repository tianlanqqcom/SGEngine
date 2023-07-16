#include "GlobalSettings.h"

EngineTools::GlobalSettings::GlobalSettings()
{
    typeHashCode = typeid(GlobalSettings).hash_code();
}

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

void EngineTools::GlobalSettings::Start()
{

}
