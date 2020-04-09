#include <iostream>

#include <SGAL/SGAL.h>

int main()
{
    using namespace sgal;

    Window window({ 1280, 720, "Coolio" });

    while (window.isOpen())
    {
        sgal::Event event;
        while (window.poll(event))
        {
            if (event.type == sgal::Event::Closed)
                window.close();
        }

        window.Update();
    }
}