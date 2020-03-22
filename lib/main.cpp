#include <iostream>
#include <cassert>

#include <vector>
#include <SGL/SGL.h>

using namespace SGL;

double randBounds(double lower = 0.0, double upper = 1.0, double p = 1)
{
    return pow((double)(rand() % 10000) / 10000.0, p) * (upper - lower) + lower;
}

struct Vector
{
    double angle, magnitude;
};

struct Map
{
    unsigned int size;

    SimplexNoise noise;
    RawModel rawModel;
    ModelArray model;

    Map(unsigned int _size) :
        size(_size), model(&rawModel)
    {
        std::vector<Vec3f> vertices;

        for (int z = 0; z < size; z++)
        {
            for (int x = 0; x < size; x++)
            {
                Vec3f position = Vec3f( x - (float)size / 2.f, 0, z - (float)size / 2.f );
                position.y = getValue({ position.x, position.z });
                vertices.push_back(position);
            }
        }

        unsigned int index = 0;
        std::vector<unsigned int> indices(6 * pow(size - 1, 2));
        for (int z = 0; z < size - 1; z++)
        {
            for (int x = 0; x < size - 1; x++)
            {
                indices[index + 0] = (z + 0) * size + (x + 0);
                indices[index + 1] = (z + 0) * size + (x + 1);
                indices[index + 2] = (z + 1) * size + (x + 1);
                
                indices[index + 3] = (z + 1) * size + (x + 0);
                indices[index + 4] = (z + 0) * size + (x + 0);
                indices[index + 5] = (z + 1) * size + (x + 1);
                
                index += 6;
            }
        }
        
        model.makeModel()/*.setScale(.5f, .5f, .5f)*/;
        rawModel.setMode(GL::Triangles);

        rawModel.loadIndices(&indices[0], indices.size());
        rawModel.loadVertices(&vertices[0], vertices.size());
        rawModel.calculateNormals(&vertices[0], vertices.size(), &indices[0], indices.size());
    }

    double getValue(Vec2f position)
    {
        float scale = 10;
        float value = sqrt(pow(position.x / scale, 2) + pow(position.y / scale, 2));
        if (value <= 1) value = 1;

        return noise.fractal(6, (position.x + 100) / 200.f, (position.y + 100) / 200.f) * 15.f / value;
    }
};

int main()
{
    Window window({ 1920, 1080 }, "Hello");
    
    Shader shader;
    shader.loadFile("shader.vert", ShaderType::Vertex);
    shader.loadFile("shader.frag", ShaderType::Fragment);
    shader.link();

    Map map(400);
    
    RawModel rawCastle;
    rawCastle.loadFromFile("post.obj");
    
    ModelArray castle(&rawCastle, true);

    const float resolution = 300.f;
    for (int i = 0; i < (int)resolution; i++)
    {
        Model* model = &castle.makeModel();

        const float angle = (float)i / resolution * 2.0 * 3.141592;
        const float magnitude = 20.f;

        model->addPosition(cos(angle) * magnitude, 0, sin(angle) * magnitude);
        model->addPosition(0, map.getValue({ model->getPosition().x, model->getPosition().z }), 0);
    }

    castle.loadModelMatrices();

    const double height = 800;
    RenderTexture texture(Vec2u( height * window.aspectRatio(), height ), true);
    
    shader.bind();
    shader.setUniform("aspectRatio", (float)texture.aspectRatio());
    shader.setUniform("min_max", Vec2f(0.001f, 1000.f));
    
    Vec2f lastMousePos = Vec2f(0, 0);
    Vec3f camera_pos = { 0.f, 0.f, 20.f };

    Clock clock;
    while (window.opened())
    {
        window.clear();

        if (Mouse::getInput(window).middle)
        {
            camera_pos.x -= (float)(Mouse::getPosition(window).x - lastMousePos.x) / 100.f;
            camera_pos.y -= (float)(Mouse::getPosition(window).y - lastMousePos.y) / 100.f;
        }
        camera_pos.z = Mouse::getInput(window).scroll / 2.0;
        lastMousePos = Mouse::getPosition(window);

        shader.bind();
        shader.setUniform("camera_vec", camera_pos);        
        shader.setUniform("use_lighting", true);

        texture.clear(Color(37.f / 255.f, 41.f / 255.f, 37.f / 255.f));
        texture.draw(castle, &shader);
        texture.draw(map.model, &shader);

        window.draw(texture);

        window.display();

        std::cout << 1.0 / clock.elapsed().seconds() << "\r";
        clock.restart();
    }

    return 0;
}

int main_old()
{
    using namespace SGL;

    Window window({ 2560, 1440 }, "Hello");

    std::vector<Vector> vertices;
    for (int i = 0; i < 5000; i++)
    {
        const double magnitude = randBounds(0.25, 4.0, 2.2);
        const double angle     = randBounds(-4.0 * 3.14159, 4.0 * 3.14159);

        vertices.push_back({ angle, magnitude });
    }

    RawModel sphereModel;
    sphereModel.loadFromFile("sphere.obj");
    Model model(&sphereModel);
    model.setScale(0.1f, 0.1f, 0.1f);

    Vector planet_pos = { 0, 1.2 };
    Model planet(&sphereModel);
    planet.setScale(0.02f);

    Shader shader;
    shader.loadFile("shader.vert", ShaderType::Vertex);
    shader.loadFile("shader.frag", ShaderType::Fragment);
    shader.link();

    const double height = 800;
    RenderTexture texture(Vec2u( height * window.aspectRatio(), height ), true);

    shader.bind();
    shader.setUniform("aspectRatio", (float)texture.aspectRatio());
    shader.setUniform("min_max", Vec2f(0.001f, 1000.f));

    Vec2f lastMousePos = Vec2f(0, 0);
    Vec3f camera_pos = { 0.f, 0.f, 2.f };

    GL::VAO vao(GL::Points);
    vao[GL::Vertices].dynamic = true;

    Clock clock;
    while (window.opened())
    {
        std::vector<Vec3f> points(vertices.size());
        for (int i = 0; i < points.size(); i++)
        {
            const double magnitude = vertices[i].magnitude;
            const double angle     = vertices[i].angle;
            vertices[i].angle += 1.0 / pow(magnitude, 2) / 600.0;

            points[i].z = (sin(angle) * magnitude);
            points[i].x = (cos(angle) * magnitude);
            points[i].y = 0.0;
        }
        vao.loadVertices(&points[0], points.size());

        planet_pos.angle += 1.0 / pow(planet_pos.magnitude, 2) / 600.0;
        planet.setPosition(Vec3f(
            cos(planet_pos.angle) * planet_pos.magnitude, 
            0, 
            sin(planet_pos.angle) * planet_pos.magnitude 
        ));
        planet.addRotation(0.001);

        // x is x angle, y is y angle, z is magnitude
        if (Mouse::getInput(window).left)
        {
            camera_pos.x -= (float)(Mouse::getPosition(window).x - lastMousePos.x) / 100.f;
            camera_pos.y -= (float)(Mouse::getPosition(window).y - lastMousePos.y) / 100.f;
        }
        camera_pos.z = Mouse::getInput(window).scroll / 25.0;
        lastMousePos = Mouse::getPosition(window);

        shader.bind();
        shader.setUniform("camera_vec", camera_pos);
        shader.setUniform("base_camera_pos", planet.getPosition());
        
        texture.clear(Color(37.f / 255.f, 41.f / 255.f, 37.f / 255.f));
        shader.setUniform("use_lighting", false);
        texture.draw(vao, &shader);
        texture.draw(model, &shader);
        shader.setUniform("use_lighting", true);
        texture.draw(planet, &shader);
        window.draw(texture);

        window.display();

        std::cout << 1.0 / clock.elapsed().seconds() << "\r";
        clock.restart();
    }

    return 0;
}