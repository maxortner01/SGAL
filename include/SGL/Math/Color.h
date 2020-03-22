#pragma once

namespace SGL
{
    struct Color
    {
        float r, g, b, a;

        Color(float _r = 1.f, float _g = 1.f, float _b = 1.f, float _a = 1.f);
    };

    Color::Color(float _r, float _g, float _b, float _a) :
        r(_r), g(_g), b(_b), a(_a)
    {   }
}