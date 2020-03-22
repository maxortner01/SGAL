#pragma once

namespace SGL
{
    struct Time
    {
        Time(double _ticks);

        double seconds() const;

    private:
        double ticks;
    };

    class Clock
    {
        double _start;

    public:
        Clock();

        void restart();

        Time elapsed() const;
    };
}