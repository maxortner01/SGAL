#include <SGAL/SGAL.h>

namespace sgal
{

    AnimationSystem::~AnimationSystem()
    {
        while (animations.size())
        {
            delete animations.front();
            animations.pop();
        }

        for (int i = 0; i < continuous_animations.size(); i++)
            delete continuous_animations[i];
    }

    void AnimationSystem::pushAnimation(Anim::BasicAnimation* animation)
    {
        animations.push(animation);
    }

    void AnimationSystem::pushContinuousAnimation(Anim::BasicAnimation* animation)
    {
        continuous_animations.push_back(animation);
    }

    void AnimationSystem::update()
    {
        if (animations.size())
        {
            animations.front()->update();
            if (animations.front()->done())
            {
                delete animations.front();
                animations.pop();
            }
        }

        for (int i = 0; i < continuous_animations.size(); i++)
        {
            continuous_animations[i]->update();
            if (continuous_animations[i]->done())
            {
                delete continuous_animations[i];
                continuous_animations.erase(continuous_animations.begin() + i--);
            }
        }
    }

}