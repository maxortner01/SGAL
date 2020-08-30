/**
 * @file   AnimationSystem.h
 * @author Max Ortner
 * @date   8-22-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

namespace sgal
{
	class SGAL_API AnimationSystem : Anim::Interface
    {
        std::vector<Anim::BasicAnimation*> continuous_animations;
        std::queue<Anim::BasicAnimation*>  animations;

    public:
        ~AnimationSystem();

        void pushAnimation(Anim::BasicAnimation* animation);
        void pushContinuousAnimation(Anim::BasicAnimation* animation);

        void update() override;
    };
}