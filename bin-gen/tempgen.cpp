/**
 * @file   tempgen.cpp
 * @author Max Ortner
 * @date   2020-04-29
 * 
 * Use this to generate the binary files used to store the default primitives for use
 * within the SGAL framework.
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include <iostream>
#include <SGAL/SGAL.h>

int main()
{
    using namespace sgal;

    DrawWindow window({ 1280, 720, "Yo" });

    RawModel rawModel;
    std::vector<Vec3f> vertices(4);
    vertices[0] = Vec3f(-1,  1, 0);
    vertices[1] = Vec3f( 1,  1, 0);
    vertices[2] = Vec3f( 1, -1, 0);
    vertices[3] = Vec3f(-1, -1, 0);
    rawModel.loadVertices(&vertices[0], vertices.size());

    Serializer::saveToFile("rect.rm", rawModel);
    return 0;
    Serializer::loadFromFile("rect.rm", rawModel);

    Vec3f* vertices_f = (Vec3f*)rawModel[GL::Vertices].readData();

    for (int i = 0; i < rawModel.vertexCount(); i++)
        std::cout << vertices_f[i].x << ", " << vertices_f[i].y << ", " << vertices_f[i].z << "\n";

    std::free(vertices_f);

    std::getchar();
}