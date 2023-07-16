#include "Component.h"
#include "GameObject.h"
#include "Components/Transform.h"

namespace EngineTools
{
    Component::Component()
    {
        typeHashCode = typeid(Component).hash_code(); // 更新类型标识符
    }

    void Component::Disable()
    {
        // 禁用组件并调用OnDisable方法
        isActive = false;
        this->OnDisable();
    }

    std::string Component::ToString(int depth) const
    {
        std::string ret = "";

        for (int i = 0; i < depth; i++)
        {
            ret += ' ';
        }
        ret += "-Component\n";

        return ret;
    }

    void Component::Enable()
    {
        // 禁用组件并调用OnDisable方法
        isActive = true;
        this->OnEnable();
    }
}

