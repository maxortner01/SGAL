#include <iostream>
#include <SGAl/SGAL.h>

#include <thread>

#include <cstring>

#include "SimplexNoise.h"
#include "SimplexNoise.cpp"

/*

    std::vector<unsigned int> indices (6);
    indices[0] = 0; indices[1] = 2; indices[2] = 1;
    indices[3] = 2; indices[4] = 0; indices[5] = 3;
    plane.rawModel.loadIndices(&indices[0], indices.size());

    std::vector<Vec3f> vertices(4);
    vertices[0] = Vec3f(-1, 0, -1);
    vertices[1] = Vec3f( 1, 0, -1);
    vertices[2] = Vec3f( 1, 0,  1);
    vertices[3] = Vec3f(-1, 0,  1);
    plane.rawModel.loadVertices(&vertices[0], vertices.size());
    
    std::vector<Vec2f> tex(4);
    tex[0] = Vec2f( 0, 1);
    tex[1] = Vec2f( 1, 1);
    tex[2] = Vec2f( 1, 0);
    tex[3] = Vec2f( 0, 0);
    plane.rawModel.loadTexCoords(&tex[0], tex.size());

    std::vector<Vec3f> normals(4);
    for (int i = 0; i < 4; i++)
        normals[i] = Vec3f(0, 1, 0);
    plane.rawModel.loadNormals(&normals[0], normals.size());
    plane.rawModel.setColor(Color(255, 255, 255));

    Serializer::saveToFile("plane.rm", plane.rawModel);
    */

using namespace sgal;

struct Chunk
{
    const unsigned int CHUNK_SIZE = 50;
    const float        BLOCK_SIZE = 2.f;
    const SimplexNoise noise;

    std::thread* thread;

    bool generating;
    bool joined;
    bool generated;
    SingleModel model;
    VertexArray array;

    Vec3i _location;

    Chunk(Vec3i location) :
        generated(false), joined(false), generating(false), _location(location), thread(nullptr)
    {
        
    }

    void start_generation()
    {
        generating = true;
        thread = new std::thread(Chunk::generate, this, _location);
    }

    void check()
    {
        if (generated && !joined)
        {
            thread->join();
            model.rawModel.fromArray(array);
            joined = true;
            array.clear();

            delete thread;
        }
    }

    static void generate(Chunk* chunk, Vec3i location)
    {
        const float THRESHOLD = 0.75f;
        const Vec3f start_pos = (Vec3f)location * ((float)chunk->CHUNK_SIZE * chunk->BLOCK_SIZE);

        for (int x = 0; x < chunk->CHUNK_SIZE; x++)
            for (int y = 0; y < chunk->CHUNK_SIZE; y++)
                for (int z = 0; z < chunk->CHUNK_SIZE; z++)
                {
                    Vec3f position = Vec3f(x, y, z) * chunk->BLOCK_SIZE + start_pos;

                    // Get the noise value
                    const float value = (chunk->noise.fractal(2, position.x / 100.f, position.y / 100.f, position.z / 100.f) + 1.f) / 2.f;

                    if (value < THRESHOLD) continue;

                    const Vec3f directions[] = {
                        Vec3f(-1, 0, 0),
                        Vec3f( 1, 0, 0),
                        Vec3f( 0,-1, 0),
                        Vec3f( 0, 1, 0),
                        Vec3f( 0, 0,-1),
                        Vec3f( 0, 0, 1)
                    };

                    for (int i = 0; i < 6; i++)
                    {
                        const Vec3f adjacent_position = position + (directions[i] * chunk->BLOCK_SIZE);
                        const float adjacent_value    = (chunk->noise.fractal(2, adjacent_position.x / 100.f, adjacent_position.y / 100.f, adjacent_position.z / 100.f) + 1.f) / 2.f;

                        Vec3f movement = directions[i] * (chunk->BLOCK_SIZE / 2.f);
                        Mat4f rotation;
                        rotation.toIdentity();

                        switch (i)
                        {
                        case 0: rotation = sgMatR({ 0.f, 0.f,  3.14159f / 2.f }); break;
                        case 1: rotation = sgMatR({ 0.f, 0.f, -3.14159f / 2.f }); break;
                        case 2: rotation = sgMatR({  3.14159f, 0.f, 0.f });       break;
                        case 4: rotation = sgMatR({ -3.14159f / 2.f, 0.f, 0.f }); break;
                        case 5: rotation = sgMatR({  3.14159f / 2.f, 0.f, 0.f }); break;
                        };

                        if (adjacent_value < THRESHOLD)
                            chunk->array.append(Primitives::getPlane().transform(sgMatS({ chunk->BLOCK_SIZE / 2.f, chunk->BLOCK_SIZE / 2.f, chunk->BLOCK_SIZE / 2.f }), rotation, sgMatT(position + movement)));
                    }
                }

        chunk->generated = true;
        chunk->generating = false;
    }
};

int main()
{
    DrawWindow window(VideoSettings(1920, 1080, "Hello"));

    std::vector<Chunk*> chunks;
    for (int x = -4; x <= 4; x++)
        for (int y = -4; y <= 4; y++)
            for (int z = -4; z <= 4; z++)
                chunks.push_back(new Chunk(Vec3f( x, y, z )));

    FPSCamera camera(3.14159f / 2.f);

    LightArray lights;
    Light mainlight;
    mainlight.type = Light::Directional;
    mainlight.position = Vec3f(-0.25f, 0.5f, 0.1f);
    lights.push(mainlight);

    RenderContext rc;
    rc.camera = &camera;
    rc.lights = &lights;
    //rc.use_lighting = false;

    Timer fpstimer;
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

        unsigned int vertices   = 0;
        unsigned int memory     = 0;
        unsigned int generating = 0;
        for (int i = 0; i < chunks.size(); i++)
        {
            if (chunks[i]->generating) generating++;
            chunks[i]->check();
            if (chunks[i]->joined)
                window.draw(chunks[i]->model, &rc);

            memory += chunks[i]->model.rawModel.getByteSize();
            vertices += chunks[i]->model.rawModel.vertexCount();
        }

        if (generating < 2)
            for (int i = 0; i < chunks.size(); i++)
                if (!chunks[i]->generated && !chunks[i]->generating)
                {
                    chunks[i]->start_generation();
                    break;
                } 
        
        if (generating < 2)
            for (int i = chunks.size() - 1; i > -1; i--)
                if (!chunks[i]->generated && !chunks[i]->generating)
                {
                    chunks[i]->start_generation();
                    break;
                } 

        window.update();

        window.setTitle("Test  |  FPS: " + std::to_string((int)(1.f / fpstimer.getElapsed())) + "  |  RAM: " + std::to_string((int)((float)memory / 1000000.f)) + " mb  |  Vertices: " + std::to_string(vertices));
        fpstimer.restart();
    }
}