#include <SGAL/SGAL.h>

#include <GL/glew.h>

#include <iostream>

namespace sgal
{

    Model::Model(RawModel const* rm) :
        rawModel(rm), normalsModel(nullptr), normalsRawModel(nullptr)
    {   }

    Model::~Model()
    {
        if (normalsRawModel)
        {
            delete normalsRawModel;
            normalsRawModel = nullptr;
        }

        if (normalsModel)
        {
            delete normalsModel;
            normalsModel = nullptr;
        }
    }

    void Model::drawNormals(const Surface& surface, RenderContext* rc)
    {
        if (!(*rawModel)[GL::Normals].getByteSize()) return;

        if (!normalsRawModel || !normalsModel)
        {
            normalsRawModel = new RawModel();

            Vec3f* const vertices = (Vec3f*)(*rawModel)[GL::Vertices].readData();
            Vec3f* const normals  = (Vec3f*)(*rawModel)[GL::Normals ].readData();

            std::vector<Color> colors(rawModel->vertexCount() * 2);
            std::vector<Vec3f> normalsVertices(rawModel->vertexCount() * 2);
            for (int i = 0; i < normalsVertices.size(); i += 2)
            {
                normalsVertices[i + 0] = *(vertices + (i / 2));
                normalsVertices[i + 1] = normalsVertices[i] + *(normals + (i / 2));

                colors[i + 0] = Color(255, 0, 0);
                colors[i + 1] = Color(255, 0, 0);
            }

            normalsRawModel->loadVertices(&normalsVertices[0], normalsVertices.size());
            normalsRawModel->loadColors(&colors[0], colors.size());
            normalsRawModel->setRenderMode(GL::Lines);

            std::free(vertices);
            std::free(normals);

            normalsModel = new Model(normalsRawModel);
        }

        normalsModel->setPosition(getPosition());
        normalsModel->setRotation(getRotation());
        normalsModel->setScale(getScale());

        bool before = rc->use_lighting;
        rc->use_lighting = false;
        surface.draw(*normalsModel, rc);

        rc->use_lighting = before;
    }

    void Model::draw(const Surface* surface, const RenderContext* rc) const
    {
        if (rawModel->vertexCount() == 0) return;

        // First step, load the model matrices into the buffer
        Mat4f modelMatrix  = getTransformMatrix();
        Mat4f normalMatrix = makeRotationMatrix(getRotation());
        rawModel->loadModelMatrices(&modelMatrix);
        rawModel->loadNormalMatrices(&normalMatrix);

        // Next step, assert whether the surface is derived from sizable
        const Sizable* ss = dynamic_cast<const Sizable*>(surface);
        SG_ASSERT(ss, "The given surface is not sizable qualified!");

        // Set the render context
        rawModel->setRenderContext(rc, ss);

        // Redudant GL calls for now
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Depth testing unless otherwise specified
        glEnable(GL_DEPTH_TEST);
        if (rc && !rc->depth_testing)
            glDisable(GL_DEPTH_TEST);

        // Bind the VAO
        rawModel->bind();

        // Bind the VBO
        (*rawModel)[GL::Vertices].bind();

        // Ascertain the rendering method
        unsigned int type;
        switch (rawModel->getRenderMode())
        {
        case GL::Triangles: type = GL_TRIANGLES;    break;
        case GL::Polygon:   type = GL_POLYGON;      break;
        case GL::Points:    type = GL_POINTS;       break;
        case GL::Lines:     type = GL_LINES;        break;
        }

        // Draw the buffer to the bound buffer
        glDrawElements(type, rawModel->indexCount(), GL_UNSIGNED_INT, rawModel->indices);

        glDisable(GL_CULL_FACE);
    }

}