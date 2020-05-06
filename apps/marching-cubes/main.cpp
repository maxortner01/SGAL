#include <SGAL/SGAL.h>
#include <iostream>
#include <cstring>
#include <thread>

#include "triangulation.cpp"

#include "../include/SimplexNoise.h"
#include "../include/SimplexNoise.cpp"

using namespace sgal;

struct Chunk
{
    std::thread* thread;
    const float        THRESHOLD  = 0.5f;
    const float        BLOCK_SIZE = .5f;
    const unsigned int CHUNK_SIZE = 24;

    bool generated, generating, joined;
    SimplexNoise noise;
    VertexArray  array;
    SingleModel  model;

    Vec3i location;

    Chunk(Vec3i _location) :
        location(_location), generated(false), generating(false), joined(false)
    {
        
    }

    void start_generation()
    {
        generating = true;
        thread = new std::thread(Chunk::generate, this);
    }

    void check()
    {
        if (generated && !joined)
        {
            thread->join();

            model.rawModel.fromArray(array);
            model.rawModel.calculateNormals();
            array.clear();

            joined = true;

            delete thread;
        }
    }

    float getValue(Vec3f position)
    {
        //return (noise.fractal(3, position.x / 75.f, position.y / 75.f, position.z / 75.f) + 1.f) / 2.f;
        float noise_value =  (noise.fractal(5, position.x / 20.f, position.y / 20.f, position.z / 20.f) + 1.f) / 2.f;
        return sqrt(dot(position, position)) - 75.f + noise_value * 5.f;
    }

    VertexArray getMarchedCube(const uint32_t index, const Vec3f position)
    {
        Vec3f directions[] = {
            Vec3f( 1.f, -1.f,  1.f),
            Vec3f( 1.f, -1.f, -1.f),
            Vec3f(-1.f, -1.f, -1.f),
            Vec3f(-1.f, -1.f,  1.f),
            Vec3f( 1.f,  1.f,  1.f),
            Vec3f( 1.f,  1.f, -1.f),
            Vec3f(-1.f,  1.f, -1.f),
            Vec3f(-1.f,  1.f,  1.f)
        };

        for (int i = 0; i < 8; i++)
            directions[i] = directions[i] * (BLOCK_SIZE / 2.f);

        VertexArray va;

        for (int i = 0; triTable[index][i] != -1; i++)
        {
            int edgeIndex = triTable[index][i];
            int points[2] = { -1, -1 };
            /**/ if (edgeIndex < 4)
            {
                points[0] = edgeIndex;
                points[1] = (edgeIndex + 1) % 4;
            }
            else if (edgeIndex >= 3 && edgeIndex < 8)
            {
                points[0] = edgeIndex;
                points[1] = ((edgeIndex - 3) % 4) + 4;
            }
            else
            {
                points[0] = edgeIndex - 8;
                points[1] = edgeIndex - 4;
            }

            float values[2] = {
                getValue(directions[points[0]] + position),
                getValue(directions[points[1]] + position)
            };

            float t = (0.5f - values[0]) / (values[1] - values[0]);
            Vec3f pos = directions[points[0]] + ((directions[points[0]] - directions[points[1]]) * t);

            Vertex vert;
            vert.position = pos * 2.f + position;
            va.push(vert);
        }

        return va;
    }

    static void generate(Chunk* chunk)
    {
        const Vec3f start_point = (Vec3f)chunk->location * ((float)chunk->CHUNK_SIZE * chunk->BLOCK_SIZE);

        const Vec3f directions[] = {
            Vec3f( 1.f, -1.f,  1.f),
            Vec3f( 1.f, -1.f, -1.f),
            Vec3f(-1.f, -1.f, -1.f),
            Vec3f(-1.f, -1.f,  1.f),
            Vec3f( 1.f,  1.f,  1.f),
            Vec3f( 1.f,  1.f, -1.f),
            Vec3f(-1.f,  1.f, -1.f),
            Vec3f(-1.f,  1.f,  1.f)
        };

        for (int x = 0; x < chunk->CHUNK_SIZE; x++)
            for (int y = 0; y < chunk->CHUNK_SIZE; y++)
                for (int z = 0; z < chunk->CHUNK_SIZE; z++)
                {
                    const Vec3f position = Vec3f(x, y, z) * (chunk->BLOCK_SIZE) + start_point;

                    float weights[8];

                    uint32_t index = 0;
                    for (int i = 0; i < 8; i++)
                    {
                        const Vec3f point_pos = position + (directions[i] * (chunk->BLOCK_SIZE / 2.f));
                        float value = chunk->getValue(point_pos);
                        if (value < chunk->THRESHOLD) continue;

                        weights[i] = value;

                        index |= 1 << i;
                    }

                    VertexArray marchedCube = chunk->getMarchedCube(index, position);
                    chunk->array.append(marchedCube.transform(sgMatT(position)));
                }

        chunk->generated  = true;
        chunk->generating = false;
    }
};

// Ray casting, draw sphere around endpoint

int main()
{
    DrawWindow window(VideoSettings(1920, 1080, "Hello"));

    int radius = 7;
    std::vector<Chunk*> chunks;
    for (int x = -radius; x <= radius; x++)
        for (int y = -radius; y <= radius; y++)
            for (int z = -radius; z <= radius; z++)
                chunks.push_back(new Chunk({ x, y, z }));

    SingleModel cube;
    cube.rawModel.fromArray(Primitives::getCube());
    cube.rawModel.setRenderMode(GL::LineStrip);

    LightArray lights;
    Light mainlight;
    mainlight.type = Light::Directional;
    mainlight.position = Vec3f(025.f, 0.5f, -1.f);
    lights.push(mainlight);

    OrbitCamera camera(3.14159f / 2.f);
    RenderContext rc;
    rc.camera = &camera;
    rc.lights = &lights;
    //rc.use_lighting = false;

    Mouse::setPosition({ (int)window.getSize().x / 2, (int)window.getSize().y / 2 });

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

        camera.update(window, 2.f, 50.f);

        window.clear(Color(100, 100, 100));

        unsigned int memory = 0, vertices = 0, generating = 0;
        for (int i = 0; i < chunks.size(); i++)
        {
            chunks[i]->check();
            if (chunks[i]->joined)
            {
                window.draw(chunks[i]->model, &rc);
                memory   += chunks[i]->model.rawModel.getByteSize();
                vertices += chunks[i]->model.rawModel.vertexCount();
            }

            if (chunks[i]->generating) generating++;
        }

        while (generating < 5)
        {
            bool none = false;
            for (int i = 0; i < chunks.size(); i++)
            {
                if (!chunks[i]->generated && !chunks[i]->generating)
                {
                    chunks[i]->start_generation();
                    generating++;
                    break;
                }
                else if (i == chunks.size() - 1) none = true;
            }

            if (none) break;

            for (int i = chunks.size() - 1; i >= 0; i--)
            {
                if (!chunks[i]->generated && !chunks[i]->generating)
                {
                    chunks[i]->start_generation();
                    generating++;
                    break;
                }
                else if (i == 0) none = true;
            }

            if (none) break;
        }

        window.update();

        window.setTitle("Test  |  FPS: " + std::to_string((int)(1.f / fpstimer.getElapsed())) + "  |  RAM: " + std::to_string((int)((float)memory / 1000000.f)) + " MB  |  Vertices: " + std::to_string(vertices));
        fpstimer.restart();
    }
}