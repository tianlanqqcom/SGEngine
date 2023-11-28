//
// Created by tianlan on 2023/5/11.
//

#include "BaseAnimation.h"

namespace EngineTools
{
    bool BaseAnimation::NextFrame()
    {
        return true;
    }

    void BaseAnimation::Restart()
    {
        // No operation
    }

    BaseAnimation* BaseAnimation::Copy()
    {
        return new BaseAnimation;
    }
} // EngineTools