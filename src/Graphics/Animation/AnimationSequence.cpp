#include <SGAL/SGAL.h>

namespace sgal
{

    AnimationSequence::~AnimationSequence()
    {
        while (animations.size())
        {
            delete animations.front();
            animations.pop();
        }
    }
    
    void AnimationSequence::pushAnimation(Anim::BasicAnimation* animation)
    {
        animations.push(animation);
    }

    bool AnimationSequence::done() const
    {
        return (animations.size() == 0);
    }

    void AnimationSequence::update()
    {
        if (!animations.size()) return;

        animations.front()->update();
        if (animations.front()->done())
        {
            delete animations.front();
            animations.pop();
        }
    }

}