#include <SGAL/SGAL.h>

#include <GL/glew.h>

#include <iostream>

namespace sgal
{

    Model::Model(RawModel const* rm) :
        rawModel(rm)
    {   }

    void Model::draw(const Surface* surface, const RenderContext* rc) const
    {
        if (rawModel->vertexCount() == 0) return;

        Mat4f modelMatrix  = getTransformMatrix();
        Mat4f normalMatrix = makeRotationMatrix(getRotation());
        rawModel->loadModelMatrices(&modelMatrix);
        rawModel->loadNormalMatrices(&normalMatrix);

        rawModel->setRenderContext(rc);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        rawModel->bind();

        (*rawModel)[GL::Vertices].bind();

        unsigned int type;
        switch (rawModel->getRenderMode())
        {
        case GL::Triangles: type = GL_TRIANGLES;    break;
        case GL::Points:    type = GL_POINTS;       break;
        case GL::Lines:     type = GL_LINES;        break;
        }

        glDrawElements(type, rawModel->indexCount(), GL_UNSIGNED_INT, rawModel->indices);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
    }

}