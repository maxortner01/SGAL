#include <SGL/SGL.h>

#include <GLFW/glfw3.h>

namespace SGL
{
    Time::Time(double _ticks) :
        ticks(_ticks)
    {   }

    double Time::seconds() const
    {
        return ticks;
    }

    Clock::Clock() :
        _start(glfwGetTime())
    {

    }

    void Clock::restart()
    {
        _start = glfwGetTime();
    }

    Time Clock::elapsed() const
    {
        return Time(glfwGetTime() - _start);
    }
}