#pragma once

namespace SGL::GL
{
    struct Object
    {
        unsigned int id;

        virtual void   bind() const = 0;
        virtual void unbind() const = 0;
    };
}