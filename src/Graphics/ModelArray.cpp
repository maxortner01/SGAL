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
    
	Model& ModelArray::operator [] (const unsigned int index)
    {
        return get(index);
    }

    unsigned int ModelArray::size() const
    {
        return models.size();
    }

    void ModelArray::draw(const Surface* surface, const RenderContext* rc) const
    {
        if (rawModel->vertexCount() == 0) return;

        // First step, load the model matrices into the buffer
        loadMatrices();

        // Next step, assert whether the surface is derived from sizable
        const Sizable* ss = dynamic_cast<const Sizable*>(surface);
        SG_ASSERT(ss, "The given surface is not sizable qualified!");

        // Set the render context
        rawModel->setRenderContext(rc, ss);

        if (rc && !rc->texture_override)
            rawModel->bindTextures();

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

        glPointSize(2.f);

        // Ascertain the rendering method
        unsigned int type;
        switch (rawModel->getRenderMode())
        {
        case GL::LineStrip: type = GL_LINE_STRIP;   break;
        case GL::Triangles: type = GL_TRIANGLES;    break;
        case GL::Polygon:   type = GL_POLYGON;      break;
        case GL::Points:    type = GL_POINTS;       break;
        case GL::Lines:     type = GL_LINES;        break;
        }

        // Draw the buffer to the bound buffer
        glDrawElementsInstanced(type, rawModel->indexCount(), GL_UNSIGNED_INT, rawModel->indices, models.size());

        glDisable(GL_CULL_FACE);
        
        if (rc && !rc->texture_override)
            rawModel->unbindTextures();
    }

}