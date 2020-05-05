#include <SGAL/SGAL.h>
#include <iostream>

#include "../include/SimplexNoise.h"
#include "../include/SimplexNoise.cpp"

using namespace sgal;

struct Chunk
{
    static const VertexArray& getCube()
    {
        static VertexArray* r = nullptr;

        if (!r)
        {
            r = new VertexArray(12);
            
            Vertex vertex;
            for (int i = 0; i < 2; i++)
            {
                float pos = (i * 2) - 1;
                vertex.position = Vec3f(pos,  1.f, 0.f);
                r->push(vertex);
                
                vertex.position = Vec3f(pos, -1.f, 0.f);
                r->push(vertex);
                
                vertex.position = Vec3f(pos,  0.f,  1.f);
                r->push(vertex);
                
                vertex.position = Vec3f(pos,  0.f, -1.f);
                r->push(vertex);
            }

            vertex.position = Vec3f(0.f, 1.f, -1.f);
            r->push(vertex);
            
            vertex.position = Vec3f(0.f, 1.f,  1.f);
            r->push(vertex);
            
            vertex.position = Vec3f(0.f, -1.f, -1.f);
            r->push(vertex);
            
            vertex.position = Vec3f(0.f, -1.f,  1.f);
            r->push(vertex);
        }

        return *r;
    }


};

int main()
{
    DrawWindow window(VideoSettings(1920, 1080, "Hello"));

    std::vector<unsigned int> indices(6);
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
    indices[3] = 0; indices[4] = 2; indices[5] = 1;
    VertexArray test;
    test = Chunk::getCube().index(&indices[0], indices.size());

    SingleModel model;
    model.rawModel.fromArray(test);
    model.rawModel.calculateNormals();

    OrbitCamera camera(3.14159f / 2.f);
    RenderContext rc;
    rc.camera = &camera;
    rc.use_lighting = false;

    while (window.isOpen())
    {
        Event event;
        while (window.poll(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyDown)
                if (event.key.code == Keyboard::Escape)
                    window.close();
        }

        camera.update(window, 0.25f, 50.f);

        window.clear(Color(100, 100, 100));

        window.draw(model, &rc);
        model.model.drawNormals(window, &rc);

        window.update();
    }
}