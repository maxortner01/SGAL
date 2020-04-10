#include <SGAL/SGAL.h>

namespace sgal
{

    Timer::Timer()
    {
        restart();
    }

    void Timer::restart()
    {
        time = clock();
    }

    float Timer::getElapsed() const
    {
        return (float)(clock() - time) / CLOCKS_PER_SEC;
    }

}