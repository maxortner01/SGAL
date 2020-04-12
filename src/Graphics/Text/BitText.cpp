#include <SGL/SGL.h>

#include <GL/glew.h>

#include <iostream>

namespace SGL
{
    void BitText::generateRawModel(const Surface* surface) const
    {
        std::vector<Vec3f> vertices;
        std::vector<Vec2f> tex;
        vertices.reserve(str.length() * 6);
        tex.reserve(vertices.capacity());

        float x_offset = 0.f;
        float aspectRatio = 1.f;
        if (surface) aspectRatio = surface->aspectRatio();

        for (const char& ch : str)
        {
            Character character = font->getCharacter(ch);
            FloatRect c = font->getCharacterRect(ch);

            character.base_width *= getScale().x;

            const float height    = (float)(font->font_height * getScale().y) / (2.f * (float)surface->getSize().y);
            const float bef_x_pos = (float)(/******************/   x_offset)  / (2.f * (float)surface->getSize().x);
            const float aft_x_pos = (float)(character.base_width + x_offset)  / (2.f * (float)surface->getSize().x);

            tex.push_back(Vec2f(c.x, c.y + c.height));
            vertices.push_back(Vec3f(bef_x_pos, 0));
            tex.push_back(Vec2f(c.x, c.y));
            vertices.push_back(Vec3f(bef_x_pos, -height));
            tex.push_back(Vec2f(c.x + c.width, c.y));
            vertices.push_back(Vec3f(aft_x_pos, -height));
            
            tex.push_back(Vec2f(c.x + c.width, c.y));
            vertices.push_back(Vec3f(aft_x_pos, -height));
            tex.push_back(Vec2f(c.x + c.width, c.y + c.height));
            vertices.push_back(Vec3f(aft_x_pos, 0));
            tex.push_back(Vec2f(c.x, c.y + c.height));
            vertices.push_back(Vec3f(bef_x_pos, 0));

            x_offset += (character.base_width * font->letter_spacing);
        }

        rawModel->loadVertices(&vertices[0], vertices.size());
        rawModel->loadTexCoords(&tex[0], tex.size());
        *changed = false;
    }

    BitText::BitText() :
        rawModel(new RawModel()), changed(new bool(false))
    {
        rawModel->setDynamic(true);
    }

    BitText::~BitText()
    {
        delete rawModel;
        delete changed;
    }

    void BitText::draw(const Surface* surface, Shader* shader, Camera* camera) const
    {
        if (!font) return;

        if (*changed)
            generateRawModel(surface);

        glDisable(GL_DEPTH_TEST);

        glActiveTexture(GL_TEXTURE0 + 1);
        font->bitmap_texture.bind();

        if (!shader) Shader::setDefault();
        else
        {
            shader->bind();
            shader->setUniform("base_texture", font->bitmap_texture);
            shader->setUniform("use_texture", true);
        }

        Model model(rawModel);
        model.setPosition(getPosition());
        model.setRotation(getRotation());
        model.setScale   (getScale()   );
        
        surface->draw(model, shader, camera);

        if (shader)
        {
            shader->bind();
            shader->setUniform("use_texture", false);
        }

        font->bitmap_texture.unbind();
        glEnable(GL_DEPTH_TEST);
        glActiveTexture(GL_TEXTURE0);
    }
}