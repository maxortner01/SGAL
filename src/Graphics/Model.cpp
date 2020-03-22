#include <SGL/SGL.h>

namespace SGL
{
    Model::Model(RawModel const* rm) :
        rawModel(rm)
    {   }

    void Model::draw(Shader* shader) const
    {
        // Set uniforms for location
        if (shader)
        {
            shader->setUniform("model_scale", getScale());
            shader->setUniform("model_rot", getRotation());
            shader->setUniform("model_trans", getPosition());
        }

        rawModel->draw(shader);
        
        if (shader)
        {
            shader->setUniform("model_scale", 1.f);
            shader->setUniform("model_rot", Vec3f(0, 0, 0));
            shader->setUniform("model_trans", Vec3f(0, 0, 0));
        }
    }
}