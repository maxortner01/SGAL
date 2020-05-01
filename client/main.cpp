#include <SGAL/SGAL.h>
#include <iostream>

#include "SimplexNoise.h"
#include "SimplexNoise.cpp"

/* CUBE

    std::vector<Vec3f> vertices(8);
    std::vector<Vec3f> realVertices;
    vertices[0] = Vec3f( 1,  1,  1);
    vertices[1] = Vec3f(-1,  1,  1);
    vertices[2] = Vec3f(-1,  1, -1);
    vertices[3] = Vec3f( 1,  1, -1);
    
    vertices[4] = Vec3f( 1, -1,  1);
    vertices[5] = Vec3f(-1, -1,  1);
    vertices[6] = Vec3f(-1, -1, -1);
    vertices[7] = Vec3f( 1, -1, -1);

    std::vector<unsigned int> indices(36);
    indices[0] = 0; indices[1] = 2; indices[2] = 1;
    indices[3] = 2; indices[4] = 0; indices[5] = 3;
    
    indices[6] = 4; indices[ 7] = 5; indices[ 8] = 6;
    indices[9] = 6; indices[10] = 7; indices[11] = 4;

    indices[12] = 0; indices[13] = 4; indices[14] = 3;
    indices[15] = 3; indices[16] = 4; indices[17] = 7;

    indices[18] = 1; indices[19] = 2; indices[20] = 5;
    indices[21] = 5; indices[22] = 2; indices[23] = 6;
    
    indices[24] = 3; indices[25] = 6; indices[26] = 2;
    indices[27] = 3; indices[28] = 7; indices[29] = 6;

    indices[30] = 0; indices[31] = 1; indices[32] = 5;
    indices[33] = 0; indices[34] = 5; indices[35] = 4;

    for (int i = 0; i < indices.size(); i++)
        realVertices.push_back(vertices[indices[i]]);

    //box.loadIndices(&indices[0], indices.size());
    //box.loadVertices(&realVertices[0], realVertices.size());
    //box.setColor(Color(255, 255, 255, 255));
    //box.calculateNormals(&realVertices[0], realVertices.size());

    //Serializer::saveToFile("cube.rm", box);

*/

int main()
{
    using namespace sgal;

    DrawWindow window({ 1920, 1080, "Test" });

    FPSCamera camera(3.14159f / 2.f);

    RawModel cube;
    Serializer::loadFromFile("cube.rm", cube);
    
    ModelArray cubes(&cube);

    SingleModel box;
    Serializer::loadFromFile("outline_cube.rm", box.rawModel);
    box.rawModel.setRenderMode(GL::Lines);
    box.rawModel.setColor(Color(0, 0, 0, 255));

    std::cout << "Cube GPU size:   " << box.rawModel.getByteSize() << " bytes\n";

    const float delta = 0.01f;
    //SingleModel points;
    SimplexNoise noise;

    SingleModel bigMesh;
    std::vector<Vec3f> bigMeshVertices;
    
    //points.model.addScale({ 5.f, 5.f, 5.f });
    box.model.addScale({ 100.f, 100.f, 100.f });

    const float threshold = 0.5f;

    unsigned int cube_count = 0;

    std::vector<Vec3f> vertices;
    std::vector<Color> colors;
    for (float x = -1; x <= 1; x += delta)
        for (float y = -1; y <= 1; y += delta)
            for (float z = -1; z <= 1; z += delta)
            {
                //const float value = (x + 1.f) / 2.f;
                const float value = (noise.fractal(4, x, y, z) + 1.f) / 2.f; 
                vertices.push_back(Vec3f(x, y, z));
                colors.push_back(Color((int)(value * 255.f), (int)(value * 255.f), (int)(value * 255.f), 255));

                if (value > threshold)
                {
                    Vec3f directions[6] = {
                        Vec3f(0,  1, 0),
                        Vec3f(0, -1, 0),
                        Vec3f( 1, 0, 0),
                        Vec3f(-1, 0, 0),
                        Vec3f(0, 0, -1),
                        Vec3f(0, 0,  1)
                    };

                    // Mesh Generation
                     
                    {

                        for (int i = 0; i < 6; i++)
                        {
                            const float size   = delta * (box.model.getScale().x);
                            const Vec3f newLoc = Vec3f(x, y, z) + (directions[i] * delta);
                            const float value2 = (noise.fractal(4, newLoc.x, newLoc.y, newLoc.z) + 1.f) / 2.f; 

                            if (value2 > threshold) continue;

                            cube_count++;

                            switch (i)
                            {
                            case 0: // top
                            {
                                Vec3f v[4] = {
                                    Vec3f(-1, 1, -1),
                                    Vec3f( 1, 1, -1),
                                    Vec3f( 1, 1,  1),
                                    Vec3f(-1, 1,  1),
                                };

                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[1] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[3] * size + Vec3f(x, y, z) * box.model.getScale().x);

                                break;
                            }
                            
                            case 1: // bottom
                            {
                                Vec3f v[4] = {
                                    Vec3f(-1, -1, -1),
                                    Vec3f( 1, -1, -1),
                                    Vec3f( 1, -1,  1),
                                    Vec3f(-1, -1,  1),
                                };

                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[1] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[3] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);

                                break;
                            }
                                
                            case 2: // right
                            {
                                Vec3f v[4] = {
                                    Vec3f( 1, -1, -1),
                                    Vec3f( 1,  1, -1),
                                    Vec3f( 1,  1,  1),
                                    Vec3f( 1, -1,  1),
                                };

                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[1] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[3] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);

                                break;
                            }
                            
                            case 3: // left
                            {
                                Vec3f v[4] = {
                                    Vec3f(-1, -1, -1),
                                    Vec3f(-1,  1, -1),
                                    Vec3f(-1,  1,  1),
                                    Vec3f(-1, -1,  1),
                                };

                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[1] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[3] * size + Vec3f(x, y, z) * box.model.getScale().x);

                                break;
                            }
                                
                            case 4: // back
                            {
                                Vec3f v[4] = {
                                    Vec3f(-1, -1, -1),
                                    Vec3f(-1,  1, -1),
                                    Vec3f( 1,  1, -1),
                                    Vec3f( 1, -1, -1),
                                };

                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[1] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[3] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);

                                break;
                            }
                            
                            case 5: // front
                            {
                                Vec3f v[4] = {
                                    Vec3f(-1, -1,  1),
                                    Vec3f(-1,  1,  1),
                                    Vec3f( 1,  1,  1),
                                    Vec3f( 1, -1,  1),
                                };

                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[1] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                
                                bigMeshVertices.push_back(v[2] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[0] * size + Vec3f(x, y, z) * box.model.getScale().x);
                                bigMeshVertices.push_back(v[3] * size + Vec3f(x, y, z) * box.model.getScale().x);

                                break;
                            }
                            }
                        }

                    }
                    

                    continue;
                    // Box Generation
                    bool covered = true;
                    for (int i = 0; i < 6; i++)
                    {
                        Vec3f newLoc = Vec3f(x, y, z) + (directions[i] * delta);

                        const float value2 = (noise.fractal(2, newLoc.x, newLoc.y, newLoc.z) + 1.f) / 2.f; 
                        if (value2 < 0.75f)
                        {
                            covered = false;
                            break;
                        }
                    }

                    if (!covered)
                    {
                        Model& model = cubes.makeModel();
                        model.setPosition(Vec3f(x, y, z) * box.model.getScale().x);
                        model.setScale(Vec3f(delta * 3.f, delta * 3.f, delta * 3.f));
                    }
                }
            }

    std::cout << cube_count << "\n";

    bigMesh.rawModel.loadVertices(&bigMeshVertices[0], bigMeshVertices.size());
    bigMesh.rawModel.calculateNormals();
    bigMesh.rawModel.setColor(Color(255, 255, 255, 255));
    //std::cout << cubes.size() << "\n";

    //points.rawModel.loadVertices(&vertices[0], vertices.size());
    //points.rawModel.loadColors(&colors[0], colors.size());
    //points.rawModel.setRenderMode(GL::Points);
    
    //std::cout << "Points GPU size: " << points.rawModel.getByteSize() << " bytes\n";

    LightArray la;
    Light light;
    light.type = Light::Directional;
    light.position = Vec3f(0.25f, 0.5f, 1.f);
    la.push(light);

    RenderContext rc;
    rc.camera = &camera;
    rc.lights = &la;
    rc.use_lighting = false;

    sgal::Timer clock;
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

        rc.use_lighting = false;
        window.draw(box, &rc);
        //window.draw(points, &rc);

        rc.use_lighting = true;
        window.draw(bigMesh, &rc);
        //window.draw(cubes, &rc);

        window.update();

        std::cout << 1.f / clock.getElapsed() << "\r"; 
        clock.restart();
    }

    return 0;
}

int main_old()
{
    using namespace sgal;

    DrawWindow window({ 1920, 1080, "2D Test" });

    RawModel chestRawModel;
    chestRawModel.fromFile("res/models/B_NtSrCompC_0_001.obj");

    Model chestModel(&chestRawModel);
    chestModel.setPosition({ 0, 0, 10 });
    chestModel.setScale(10.f, 10.f, 10.f);

    LightArray lightArray;
    Light main_light;
    main_light.type     = Light::Directional;
    main_light.position = Vec3f(1.f, 0.25f, -1.f);

    lightArray.push(main_light);

    FPSCamera camera(3.14159f / 2.f);

    RenderContext rc;
    rc.camera = &camera;
    rc.lights = &lightArray;
    rc.shader = &Shader::Default3D();

    UI::Rectangle rectangle;
    rectangle.setSize({ 200.f, 200.f });
    rectangle.setColor(Color(255, 0, 0, 255));
    rectangle.setPosition({ 10.f, 10.f });
    rectangle.setRadius(10.f);
    
    DrawTexture test(Vec2u{ 180, 180 });

    UI::Rectangle texture_test;
    texture_test.setParent(&rectangle);
    texture_test.setSize(rectangle.getSize() + Vec2f(-rectangle.getRadius() * 2.f, -rectangle.getRadius() * 2.f));
    texture_test.setSize({ texture_test.getSize().x + 20.f, texture_test.getSize().x + 20.f });
    texture_test.setColor(Color(255, 255, 255, 255));
    texture_test.setTexture(test.texture());
    texture_test.setRadius(10.f); 
    texture_test.setPosition({ -10.f, -10.f, 0.f });

    unsigned int index = 0;
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

        camera.update(window, 1.f, 50.f);

        window.clear(Color(75, 75, 75));

        window.draw(chestModel, &rc);
        chestModel.drawNormals(window, &rc);

        test.clear(Color(0, 0, 0, 0));
        test.draw(chestModel, &rc);
        chestModel.drawNormals(test, &rc);

        chestModel.addRotation({ 0, 0.01f, 0 });

        window.draw(rectangle);
        window.draw(texture_test);

        window.update();
        index++;
    }

    return 0;
}