/**
 * @file   Animation.h
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
	template<typename T>
	class SGAL_API Animation : public Anim::BasicAnimation
	{
    public:
        enum Interpolation
        {
            Linear,
            Cosine
        };

    private:
        T* value;
        T  first, last;
        float duration;

        Timer timer;

        bool started = false;
        bool finished;

        Interpolation interp = Cosine;

    public:

        Animation(T* _value, T _first, T _last, float _duration) :
            value(_value), first(_first), last(_last), duration(_duration)
        { finished = false; }

        void setInterpolation(Interpolation i)
        {
            interp = i;
        }

        bool done() const override
        {
            return finished;
        }

        void update() override
        {
            if (!started) { timer.restart(); started = true; }

            float x_value = timer.getElapsed() / duration;

            if (x_value >= 1)
            {
                finished = true; return;
            }

            // Interpolate
            switch (interp)
            {
            case Linear:
                *value = first + (last - first) * x_value;
                return;
            
            case Cosine:
                *value = first + (last - first) * -0.5f * (cosf(3.14159f * x_value) - 1.f);
                return;
            }

        }
    };
}