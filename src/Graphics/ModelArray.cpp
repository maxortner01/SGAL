#include <SGL/SGL.h>

#include <GL/glew.h>

namespace SGL
{
    void ModelArray::loadModelMatrices() const
    {
        std::vector<Mat4f> modelMatrices;
        modelMatrices.reserve(models.size());

        for (int i = 0; i < models.size(); i++)
        {
            Mat4f modelMatrix = 
                makeScaleMatrix(models[i]->getScale()) *
                makeRotationMatrix(models[i]->getRotation()) *
                makeTranslationMatrix(models[i]->getPosition());

            modelMatrices.push_back(modelMatrix);
            
            /*
            for (int r = 0; r < modelMatrix.getRows(); r++)
            {
                for (int c = 0; c < modelMatrix.getColumns(); c++)
                    std::cout << modelMatrix(r, c) << " ";
                std::cout << "\n";
            } */
        }
        
        rawModel->loadModelMatrices(&modelMatrices[0], modelMatrices.size());
    }

    ModelArray::ModelArray(RawModel const* rm, bool _static_render) :
        rawModel(rm), static_render(_static_render)
    {   }

    ModelArray::~ModelArray()
    {
        for (int i = 0; i < models.size(); i++)
            delete models[i];
    }

    Model& ModelArray::makeModel()
    {
        models.push_back(new Model(rawModel));
        return *models[models.size() - 1];
    }

    void ModelArray::remove(unsigned int index)
    {
        models.erase(models.begin() + index);
    }

    Model& ModelArray::get(unsigned int index) const
    {
        return *models[index];
    }

    void ModelArray::draw(Shader* shader) const
    {
        if (!static_render)
            loadModelMatrices();

        shader->bind();
        rawModel->bind();
        
        rawModel->vbos[GL::Vertices]->bind();

        unsigned int type;
        switch (rawModel->renderMode)
        {
        case GL::Triangles: type = GL_TRIANGLES;    break;
        case GL::Points:    type = GL_POINTS;       break;
        case GL::Lines:     type = GL_LINES;        break;
        };

        // Draw the array to the context
        glDrawElementsInstanced(type, rawModel->getIndexCount(), GL_UNSIGNED_INT, rawModel->indices, models.size());
    }
}