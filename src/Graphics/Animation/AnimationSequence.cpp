#include <SGAL/SGAL.h>

namespace sgal
{

    AnimationSequence::~AnimationSequence()
    {
        while (animations.size())
        {
            delete animations.top();
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

        animations.top()->update();
        if (animations.top()->done())
        {
            delete animations.top();
            animations.pop();
        }
    }

}