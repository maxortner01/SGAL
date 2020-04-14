#include <SGAL/SGAL.h>

#include <GL/glew.h>

namespace sgal
{

    ModelArray::ModelArray(RawModel const* rm, bool _static) :
        rawModel(rm), static_render(_static)
    {   }

    Model& ModelArray::makeModel()
    {
        models.push_back(Model(rawModel));
        return models[models.size() - 1];
    }

    void ModelArray::loadMatrices() const
    {
        std::vector<Mat4f> modelMatrices (models.size());
        std::vector<Mat4f> normalMatrices(models.size());

        for (int i = 0; i < models.size(); i++)
        {
            modelMatrices [i] = models[i].getTransformMatrix();
            normalMatrices[i] = makeRotationMatrix(models[i].getRotation());
        }

        rawModel->loadModelMatrices (&modelMatrices[0],  modelMatrices.size());
        rawModel->loadNormalMatrices(&normalMatrices[0], normalMatrices.size());
    }

    void ModelArray::setStaticRender(bool _static)
    {
        static_render = _static;
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

        if (!static_render)
            loadMatrices();

        Model::setRenderContext(rc);

        glEnable(GL_DEPTH_TEST);
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

        glDrawElementsInstanced(type, rawModel->indexCount(), GL_UNSIGNED_INT, rawModel->indices, models.size());

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
    }

}