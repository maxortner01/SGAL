#include <SGL/SGL.h>

namespace SGL
{
    Model::Model(RawModel const* rm) :
        rawModel(rm)
    {   }

    void Model::draw(const Surface* surface, Shader* shader, Camera* camera) const
    {
        Mat4f modelMatrix = getTransformMatrix();
        Mat4f normMatrix  = makeRotationMatrix(getRotation());

        rawModel->loadModelMatrices(&modelMatrix, 1);
        rawModel->loadNormalMatrices(&normMatrix, 1);

        rawModel->draw(surface, shader, camera);
    }
}