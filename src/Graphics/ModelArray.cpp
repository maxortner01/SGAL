#include <SGAL/SGAL.h>

#include <GL/glew.h>

namespace sgal
{

    void ModelArray::loadMatrices() const
    {
        std::vector<Mat4f> modelMatrices (models.size());
        std::vector<Mat4f> normalMatrices(models.size());

        for (int i = 0; i < modelMatrices.size(); i++)
        {
            modelMatrices [i] = models[i].getTransformMatrix();
            normalMatrices[i] = makeRotationMatrix(models[i].getRotation());
        }

        rawModel->loadModelMatrices (&modelMatrices[0],  modelMatrices.size());
        rawModel->loadNormalMatrices(&normalMatrices[0], normalMatrices.size());
    }

    ModelArray::ModelArray(RawModel const* rm) :
        rawModel(rm)
    {   }

    Model& ModelArray::makeModel()
    {
        models.push_back(Model(rawModel));
        return models[models.size() - 1];
    }

    void ModelArray::remove(const unsigned int index)
    {
        models.erase(models.begin() + index);
    }

    Model& ModelArray::get(const unsigned int index)
    {
        return models[index];
    }

    void ModelArray::draw(const Surface* surface, const RenderContext* rc) const
    {
        if (rawModel->vertexCount() == 0) return;

        loadMatrices();

        if (rc)
        {
            if (rc->shader)
            {
                rc->shader->bind();
                if (rc->camera)
                {
                    rc->shader->setUniform("view_matrix", rc->camera->getPerspectiveMatrix());
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
        case GL::Triangles: type = GL_TRIANGLES; break;
        case GL::Points:    type = GL_POINTS;    break;
        case GL::Lines:     type = GL_LINES;     break;
        };

        glDrawElementsInstanced(type, models.size(), GL_UNSIGNED_INT, rawModel->indices, rawModel->indexCount());

        glEnable(GL_BLEND);
    }

}