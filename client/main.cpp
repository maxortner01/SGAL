#include <iostream>
#include <SGAl/SGAL.h>

#include <cstring>

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

int main()
{
    using namespace sgal;

    DrawWindow window(VideoSettings(1920, 1080, "Hello"));

    Texture atlas;
    atlas.fromFile("res/textures/atlas.png");

    VertexArray planeArray;
    Serializer::loadFromFile("plane.rm", planeArray);

    std::cout << planeArray.size() << "\n";

    RawModel planeRawModel(planeArray);

    Model plane(&planeRawModel);

    OrbitCamera camera(3.14159f / 2.f);
    camera.setPosition({ 0, 0, -10 });
    camera.addRotation({ -3.14159f / 4.f, -3.14159f / 4.f, 0 });

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

        window.draw(plane, &rc);

        window.update();
    }
}