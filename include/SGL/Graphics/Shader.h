#pragma once

#include "../Math.h"
#include "GL/GLData.h"
#include <string>

namespace SGL
{
    enum class ShaderType
    {
        Vertex,
        Fragment,
        Count
    };

    class Shader : GL::Object
    {
        unsigned int ids[(int)ShaderType::Count];

    public:
        Shader();
        ~Shader();

        void setUniform(const std::string& name, Vec3f value) const;
        void setUniform(const std::string& name, Vec2f value) const;
        void setUniform(const std::string& name, float value) const;
        void setUniform(const std::string& name, bool  value) const;

        void loadFile(const std::string& location, ShaderType type);
        void link() const;

        void   bind() const override;
        void unbind() const override {}

        static void setDefault();
    };
}