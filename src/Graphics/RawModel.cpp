#include <SGL/SGL.h>

#include <fstream>
#include <cassert>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobj/tiny_obj_loader.h>

namespace SGL
{
    void RawModel::loadFromFile(const std::string& location)
    {
        using namespace tinyobj;

        std::vector<Vec3f> vertices, normals;
        std::vector<Vec2f> tex;

        attrib_t attrib;
        std::vector<shape_t> shapes;
        std::vector<material_t> materials;

        std::string warn, err;

        bool ret = LoadObj(&attrib, &shapes, &materials, &warn, &err, location.c_str());

        if (!warn.empty())
            std::cout << "OBJ Warning: " << warn << "\n";

        if (!err.empty())
            std::cout << "OBJ Error: " << err << "\n";

        if (!ret)
        {
            std::cout << "File '" << location << "' does not exist!\n";
            return; 
        }

        vertices.reserve(attrib.GetVertices().size());
        normals.reserve(attrib.GetVertices().size());
        tex.reserve(attrib.GetVertices().size());

        for (int s = 0; s < shapes.size(); s++)
        {
            int index_offset = 0;
            for (int f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
            {
                int fv = shapes[s].mesh.num_face_vertices[f];

                for (int v = 0; v < fv; v++)
                {
                    index_t idx = shapes[s].mesh.indices[index_offset + v];

                    Vec2f tex_coord;
                    Vec3f vertex, normal;

                    if (attrib.vertices.size() > 0)
                    {
                        vertex.x = attrib.vertices[3 * idx.vertex_index + 0];
                        vertex.y = attrib.vertices[3 * idx.vertex_index + 1];
                        vertex.z = attrib.vertices[3 * idx.vertex_index + 2];
                        vertices.push_back(vertex);
                    }

                    if (attrib.normals.size() > 0)
                    {
                        normal.x = attrib.normals[3 * idx.normal_index + 0];
                        normal.y = attrib.normals[3 * idx.normal_index + 1];
                        normal.z = attrib.normals[3 * idx.normal_index + 2];
                        normals.push_back(normal);
                    }

                    if (attrib.texcoords.size() > 0)
                    {
                        tex_coord.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                        tex_coord.x = attrib.texcoords[2 * idx.texcoord_index + 1];
                        tex.push_back(tex_coord);
                    }
                }

                index_offset += fv;
            }
        }

        if (vertices.size() > 0)
            loadVertices(&vertices[0], vertices.size());

        if (normals.size() > 0)
            loadNormals(&normals[0], normals.size());

        if (tex.size() > 0)
            loadTexCoords(&tex[0], tex.size());
    }

    void RawModel::calculateNormals(const Vec3f* data, size_t count, const unsigned int* indices, size_t index_count) const
    {
        std::vector<Vec3f> normals(count);

        unsigned int real_count = (indices)?(index_count):(count);

        for (int i = 0; i < real_count; i += 3)
        {
            Vec3f v1, v2, v3;

            if (indices)
            {
                v1 = data[indices[i + 0]];
                v2 = data[indices[i + 1]];
                v3 = data[indices[i + 2]];
            }
            else
            {
                v1 = data[i + 0];
                v2 = data[i + 1];
                v3 = data[i + 2];
            }
            
            Vec3f rel_v2 = v2 - v1;
            Vec3f rel_v3 = v3 - v1;

            if (indices)
            {
                normals[indices[i + 0]] += cross(rel_v3, rel_v2);
                normals[indices[i + 1]] += cross(rel_v3, rel_v2);
                normals[indices[i + 2]] += cross(rel_v3, rel_v2);
                
            }
            else
            {
                normals[i + 0] += cross(rel_v3, rel_v2);
                normals[i + 1] += cross(rel_v3, rel_v2);
                normals[i + 2] += cross(rel_v3, rel_v2);
            }
            
        }
        
        for (int i = 0; i < count; i++)
            normals[i] = normalize(normals[i]);
        
        loadNormals(&normals[0], normals.size());
    }
}