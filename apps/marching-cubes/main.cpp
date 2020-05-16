#include <SGAL/SGAL.h>
#include <iostream>
#include <cstring>
#include <thread>
#include <math.h>

#include "triangulation.cpp"
#include "marching_cubes.cpp"

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

    float gen_time = 0.f;

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
            array.clear();

            joined = true;

            delete thread;
        }
    }

    float getHeight(Vec3f position)
    {
        return pow(noise.fractal(6, position.x / 120.f, position.y / 120.f, position.z / 120.f), 2) * 10.f;
    }

    float getValue(Vec3f position)
    {
        return sqrt(dot(position, position)) - 75.f - getHeight(position);
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

        uint32_t size;
        for (size = 0; triTable[index][size++] != -1;);

        VertexArray va(size);

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

            Color color = Color(255, 255, 255, 255);

            float height = getHeight(pos + position);
            height = (height > 1)?1:height;
            if (height < 0.501f)
            {
                float color_scale = height;
                if (color_scale < 0.005f) color_scale = 0.005f;
                color = Color(0, 0, (int)((float)255 * pow(color_scale, 0.25)), 255);
            }

            Vertex vert;
            vert.position = pos;
            vert.color = color;
            va.push(vert);
        }

        return va;
    }

    static void generate(Chunk* chunk)
    {
        Timer timer;
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

        //chunk->array.resize(pow(chunk->CHUNK_SIZE, 3) * 12);

        for (int x = 0; x < chunk->CHUNK_SIZE; x++)
            for (int y = 0; y < chunk->CHUNK_SIZE; y++)
                for (int z = 0; z < chunk->CHUNK_SIZE; z++)
                {
                    const Vec3f position = Vec3f(x, y, z) * (chunk->BLOCK_SIZE) + start_point;

                    uint32_t index = 0;
                    for (int i = 0; i < 8; i++)
                    {
                        const Vec3f point_pos = position + (directions[i] * (chunk->BLOCK_SIZE / 2.f));
                        float value = chunk->getValue(point_pos);
                        if (value < chunk->THRESHOLD) continue;

                        index |= 1 << i;
                    }

                    VertexArray marchedCube = chunk->getMarchedCube(index, position);
                    chunk->array.append(marchedCube.transform(sgMatT(position)));
                }

        chunk->array.calculateNormals();
        chunk->generated  = true;
        chunk->generating = false;

        chunk->gen_time = timer.getElapsed();
    }
};

// Make one vbo with stride [!!!!!!!!!!!!]
// Ray casting, draw sphere around endpoint

int main_old()
{
    std::vector<float> data;
    std::vector<Vec3f> vertices;
    std::vector<Color> colors;

    for (int x = 0; x < 10; x++)
        for (int y = 0; y < 10; y++)
            for (int z = 0; z < 10; z++)
            {
                const Vec3f position = Vec3f(x, y, z);

                data.push_back(sqrt(dot(position, position)) / 2.f);

                //if (x < 5 && y == 0 && z < 5) data.push_back(0.f);
                //else data.push_back(1.f);

                //if (x < 9 && y < 9 && z < 9)
                //{
                //    colors.push_back(Color(255, 0, 0));
                //    vertices.push_back(Vec3f(x, y, z) + Vec3f(0.5f, 0.5f, 0.5f));
                //}

                vertices.push_back(Vec3f(x, y, z));

                if (data[data.size() - 1] > 0.75f)
                    colors.push_back(Color(0, 0, 0));
                else
                    colors.push_back(Color(0, 255, 0));
            }

    DrawWindow window(VideoSettings(1920, 1080, "Hello"));

    marching::DataMesh<float> grid(&data[0], Vec3f(10, 10, 10), 0.75f);
    grid.generate();

    SingleModel points;
    points.rawModel.loadVertices(&vertices[0], vertices.size());
    points.rawModel.loadColors(&colors[0], colors.size());
    points.rawModel.setRenderMode(GL::Points);

    LightArray lights;
    Light light;
    light.type = Light::Directional;
    light.position = Vec3f(1, 0, 0);
    lights.push(light);

    OrbitCamera camera(3.14159f / 2.f);
    RenderContext rc;
    rc.camera = &camera;
    rc.lights = &lights;
    rc.use_lighting = true;

    while (window.isOpen())
    {
        Event event;
        while (window.poll(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyDown)
            {
                if (event.key.code == Keyboard::Escape)
                    window.close();
            }
        }

        camera.update(window, 0.25f, 50.f);

        window.clear(Color(100, 100, 100));

        rc.use_lighting = true;
        window.draw(grid, &rc);
        //grid.getData()->model.model.drawNormals(window, &rc);
        
        rc.use_lighting = false;
        window.draw(points, &rc);

        window.update();
    }
    
}

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
    mainlight.position = normalize(Vec3f(1.f, 1.f, 0.f));
    mainlight.color = Color(255, 255, 255, 255);
    lights.push(mainlight);

    OrbitCamera camera(3.14159f / 2.f);
    camera.setOrtho(true);
    RenderContext rc;
    rc.camera = &camera;
    rc.lights = &lights;

    Mouse::setPosition({ (int)window.getSize().x / 2, (int)window.getSize().y / 2 });

    bool draw_normals = false;

    Timer fpstimer;
    while (window.isOpen())
    {
        Event event;
        while (window.poll(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyDown)
            {
                if (event.key.code == Keyboard::Escape)
                    window.close();
                if (event.key.code == Keyboard::Tab)
                {
                    if (draw_normals) draw_normals = false;
                    else draw_normals = true;
                }
            }
        }

        camera.update(window, 2.f, 50.f);

        window.clear(Color(100, 100, 100));

        float total_elapsed = 0.f;
        unsigned int total_generated = 0;

        unsigned int memory = 0, vertices = 0, generating = 0;
        for (int i = 0; i < chunks.size(); i++)
        {
            chunks[i]->check();
            if (chunks[i]->joined)
            {
                rc.use_lighting = true;
                window.draw(chunks[i]->model, &rc);

                if (draw_normals)
                    chunks[i]->model.model.drawNormals(window, &rc);

                memory   += chunks[i]->model.rawModel.getByteSize();
                vertices += chunks[i]->model.rawModel.vertexCount();
            }

            if (chunks[i]->generated)
            {
                total_elapsed += chunks[i]->gen_time;
                total_generated++;
            }

            if (chunks[i]->generating) generating++;
        }

        const int total_generating = 3;

        while (generating < total_generating)
        {
            unsigned int count = 0;
            bool none = false;
            for (int i = 0; i < chunks.size() && count <= total_generating / 4; i++)
            {
                if (!chunks[i]->generated && !chunks[i]->generating)
                {
                    chunks[i]->start_generation();
                    generating++;
                    count++;
                }
                else if (i == chunks.size() - 1) none = true;
            }

            if (none) break;

            count = 0;
            for (int i = chunks.size() - 1; i >= 0 && count <= total_generating / 4; i--)
            {
                if (!chunks[i]->generated && !chunks[i]->generating)
                {
                    chunks[i]->start_generation();
                    generating++;
                    count++;
                }
                else if (i == 0) none = true;
            }

            if (none) break;
        }

        window.update();

        window.setTitle("Test  |  FPS: " + std::to_string((int)(1.f / fpstimer.getElapsed())) + "  |  RAM: " + std::to_string((int)((float)memory / 1000000.f)) + " MB  |  Vertices: " + std::to_string(vertices) + "  |  Average Gen Time: " + std::to_string(total_elapsed / (float)total_generated));
        fpstimer.restart();
    }
}