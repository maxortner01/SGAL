#pragma once

#include <iostream>

#include "../Drawable.h"
#include "BitmapFont.h"

namespace SGL
{
    class BitText : public Drawable, public Transform
    {
        RawModel*   rawModel;
        bool* changed;

        BitmapFont* font;
        std::string str;

        void generateRawModel(const Surface* surface) const;

    public:
        BitText();
        ~BitText();

        void draw(const Surface* surface = nullptr, Shader* shader = nullptr, Camera* camera = nullptr) const override;

        void setString(const std::string& string) 
        { 
            if (str != string) 
                *changed = true;

            str = string; 
        }
        void setFont(BitmapFont& bfont) { font = &bfont; }
    };
}