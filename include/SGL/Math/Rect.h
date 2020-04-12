#pragma once

namespace SGL
{
    template<typename T>
    struct Rect
    {
        T x, y, width, height;
    };

    typedef Rect<float> FloatRect;
    typedef Rect<int>   IntRect;
}