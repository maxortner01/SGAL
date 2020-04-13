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

        if (rc)
        {
            // If there's a shader
            if (rc->shader)
            {
                rc->shader->bind();
                
                if (rc->camera)
                {
                    rc->shader->setUniform("view_matrix", rc->camera->getTransformMatrix());
                    rc->shader->setUniform("proj_matrix", rc->camera->getProjectionMatrix());
                }
                else
                {
                    Mat4f identity; identity.toIdentity();
                    rc->shader->setUniform("view_matrix", identity);
                    rc->shader->setUniform("proj_matrix", identity);
                }
            }
            else
                Shader::useDefault();
        }

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

        glEnable(GL_BLEND);
    }

}