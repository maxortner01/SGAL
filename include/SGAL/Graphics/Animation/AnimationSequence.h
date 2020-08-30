/**
 * @file   AnimationSequence.h
 * @author Max Ortner
 * @date   8-22-2020
 * 
 * Description...
 * 
 * @copyright Copyright (c) 2020
**/

#pragma once

#include <SGAL/decl.h>

#include "Anim.h"

namespace sgal
{
	class SGAL_API AnimationSequence : public Anim::BasicAnimation
	{
        std::queue<Anim::BasicAnimation*> animations;

    public:
        ~AnimationSequence();

        void pushAnimation(Anim::BasicAnimation* animation);

        bool done()   const override;
        void update() override;
    };
}