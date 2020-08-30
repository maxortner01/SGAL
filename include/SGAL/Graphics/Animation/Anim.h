#pragma once

namespace sgal
{
namespace Anim
{
    struct Interface
    {
        virtual void update() = 0;
    };

    struct BasicAnimation : Interface
    {
        virtual bool done() const = 0;
    };
}   
}
