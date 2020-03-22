#pragma once

#include "GLData.h"

namespace SGL::GL
{
    struct VBO : Object
    {
        VBO(unsigned int _index);
        ~VBO();

        virtual void loadData(const void* data, size_t size, size_t count, size_t members) const;
        virtual void bind() const override;

        void unbind() const override;
        
        bool dynamic;

    protected:
        unsigned int index;
    };

    struct IndexBuffer : VBO
    {
        void loadData(const void* data, size_t size, size_t count, size_t members) const;
        void bind() const override;
    };
}