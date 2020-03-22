#pragma once

#include <string>
#include "Surface.h"

namespace SGL
{
    class Window : public Surface
    {
        friend class Mouse;

        void* handle;

    public:
        Window(Vec2u dimensions, const std::string& title = "");
        ~Window();

        void display() const;
        bool opened() const;

        void bind() const override;
    };
}