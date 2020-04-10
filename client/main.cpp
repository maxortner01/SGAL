#include <iostream>

#include <SGAL/SGAL.h>

int main()
{
    using namespace sgal;

    Window window({ 1280, 720, "Coolio" });

    Timer clock;
    while (window.isOpen())
    {
        sgal::Event event;
        while (window.poll(event))
        {
            if (event.type == sgal::Event::Closed)
                window.close();
        }

        window.clear();

        window.update();

        float fps = 1.f / clock.getElapsed();
        clock.restart();
    }
}