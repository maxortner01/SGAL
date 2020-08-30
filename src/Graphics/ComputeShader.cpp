#include <SGAL/SGAL.h>

#include <GL/glew.h>

namespace sgal
{

    void ComputeShader::dispatch(const Vec3i size) const
    {
        bind();
        glDispatchCompute(size.x, size.y, size.z);
        unbind();
    }

}