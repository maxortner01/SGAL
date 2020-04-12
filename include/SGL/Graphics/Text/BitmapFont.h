#pragma once

#include <string>

#include "../Math.h"
#include "Font.h"

namespace SGL
{
    struct Character
    {
        unsigned int base_width, width_offset, X_offset, Y_offset;
    };

    class BitmapFont : public Font
    {
        friend class BitText;

        Vec2u image_size, cell_size;
        int start_character, font_height;
        float letter_spacing = 0.75f;

        Texture bitmap_texture;
        std::vector<Character> characters;

    public:
        BitmapFont(const std::string& font_name, const std::string& directory = "");
        FloatRect getCharacterRect(char character) const;
        const Character& getCharacter(char character) const;

        void setLetterSpacing(float f) { letter_spacing = f; }
    };
}