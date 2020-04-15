#include <SGAL/SGAL.h>

#include <iostream>
#include <vector>

#define  TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tinyobjloader.h>

namespace sgal
{

    RawModel::RawModel() :
        GL::ArrayObject(GL::Triangles), use_textures(false)
    {   }

    void RawModel::fromFile(const std::string& filename)
    {
        using namespace tinyobj;

        std::vector<Vec3f> vertices, normals;
        std::vector<Vec2f> tex;

        attrib_t attrib;
        std::vector<shape_t> shapes;
        std::vector<material_t> materials;

        std::string warn, err;

        bool ret = LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

        if (!warn.empty())
            std::cout << "OBJ Warning: " << warn << "\n";

        if (!err.empty())
            std::cout << "OBJ Error: " << err << "\n";

        if (!ret)
        {
            std::cout << "File '" << filename << "' does not exist!\n";
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

        //if (tex.size() > 0)
        //    loadTexCoords(&tex[0], tex.size());
    }

    void RawModel::calculateNormals(const Vec3f* vertices, const size_t vertexCount, const unsigned int* indices, const size_t indexCount) const
    {
        if (!vertexCount) return;

        std::vector<Vec3f> normals(vertexCount);

        unsigned int real_count = (indices)?indexCount:vertexCount;

        for (int i = 0; i < real_count; i += 3)
        {
            Vec3f v1, v2, v3;

            if (indices)
            {
                v1 = vertices[indices[i + 0]];
                v2 = vertices[indices[i + 1]];
                v3 = vertices[indices[i + 2]];
            }
            else
            {
                v1 = vertices[i + 0];
                v2 = vertices[i + 1];
                v3 = vertices[i + 2];
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
        
        for (int i = 0; i < vertexCount; i++)
            normals[i] = normalize(normals[i]);

        loadNormals(&normals[0], normals.size());
    }

    
    void RawModel::setRenderContext(const RenderContext* rc) const
    {
        if (rc)
        {
            // If there's a shader
            if (rc->shader)
            {
                rc->shader->bind();

                rc->shader->setUniform("use_textures", use_textures);
                
                if (rc->camera)
                {
                    rc->shader->setUniform("vp_matrix",   rc->camera->getProjectionMatrix() * rc->camera->getPerspectiveMatrix());
                    rc->shader->setUniform("view_matrix", rc->camera->getPerspectiveMatrix());
                    rc->shader->setUniform("proj_matrix", rc->camera->getProjectionMatrix());
                }
                else
                {
                    Mat4f identity; identity.toIdentity();
                    rc->shader->setUniform("vp_matrix",   identity);
                    rc->shader->setUniform("view_matrix", identity);
                    rc->shader->setUniform("proj_matrix", identity);
                }

                if (rc->lights)
                {
                    rc->shader->setUniform(&(*rc->lights)[0], rc->lights->size());
                }
            }
            else
                Shader::useDefault();
        }
    }

}