#include <SGL/SGL.h>

#include <iostream>

namespace SGL
{
    BitmapFont::BitmapFont(const std::string& font, const std::string& directory)
    {
        bitmap_texture.loadFromFile(directory + "/" + font + ".bmp");
        CSVFile file(directory + "/" + font + ".csv");

        for (int i = 0; i < 10; i++)
        {
            /**/ if (file.getData()[i][0] == "Image Width")
                image_size.x = std::stoi(file.getData()[i][1]);
                
            else if (file.getData()[i][0] == "Image Height")
                image_size.y = std::stoi(file.getData()[i][1]);

            else if (file.getData()[i][0] == "Cell Width")
                cell_size.x = std::stoi(file.getData()[i][1]);
                
            else if (file.getData()[i][0] == "Cell Height")
                cell_size.y = std::stoi(file.getData()[i][1]);

            else if (file.getData()[i][0] == "Start Char")
                start_character = std::stoi(file.getData()[i][1]);
                
            else if (file.getData()[i][0] == "Font Height")
                font_height = std::stoi(file.getData()[i][1]);
        }

        const unsigned int character_count = (image_size.x / cell_size.x) * (image_size.y / cell_size.y);
        characters.resize(character_count);

        for (int i = 0; i < file.getData().size(); i++)
        {
            std::vector<std::string> split = file.splitLine(file.getData()[i][0], ' ');

            if (split[0] == "Char")
            {
                const unsigned int character_number = std::stoi(split[1]);
                if (character_number >= character_count || split.size() != 4) continue;

                /**/ if (split[2] + split[3] == "BaseWidth")
                    characters[character_number].base_width = std::stoi(file.getData()[i][1]);

                else if (split[2] + split[3] == "WidthOffset")
                    characters[character_number].width_offset = std::stoi(file.getData()[i][1]);
                    
                else if (split[2] + split[3] == "XOffset")
                    characters[character_number].X_offset = std::stoi(file.getData()[i][1]);
                    
                else if (split[2] + split[3] == "YOffset")
                    characters[character_number].Y_offset = std::stoi(file.getData()[i][1]);

            }
        }
    }

    FloatRect BitmapFont::getCharacterRect(char character) const
    {
        FloatRect ret;

        unsigned int character_number = ((int)character) - start_character;
        Character c = getCharacter(character);

        Vec2f texturePos = Vec2f(
            (character_number % (image_size.x / cell_size.x)) * cell_size.x,
            (character_number / (image_size.x / cell_size.x) + 1) * cell_size.y
        );
        
        ret.x = texturePos.x; ret.y = texturePos.y;
        ret.width  = c.base_width;
        ret.height = cell_size.y;

        ret.y = -ret.y + image_size.y;
        ret.x      /= (float)image_size.x;
        ret.y      /= (float)image_size.y;
        ret.width  /= (float)image_size.x;
        ret.height /= (float)image_size.y;

        return ret;
    }

    const Character& BitmapFont::getCharacter(char character) const
    {
        return characters[((int)character) - start_character];
    }
}