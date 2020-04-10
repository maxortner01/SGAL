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
            {
                std::cout << "closing\n";
                window.close();
            }

            if (event.type == sgal::Event::Resize)
                std::cout << "resize " << event.size.width << " x " << event.size.height << "\n";

            if (event.type == sgal::Event::Moving)
                std::cout << "move to " << event.position.x << " x " << event.position.y << "\n";

            if (event.type == sgal::Event::KeyDown)
                std::cout << "Key Down: " << (char)event.key.code << "\n";
            
            if (event.type == sgal::Event::KeyUp)
                std::cout << "Key Up: " << (char)event.key.code << "\n";

            if (event.type == sgal::Event::MouseDown)
                std::cout << "Mouse Down: " << (event.mouse.code == Mouse::Key_LEFT) << "\n";
                
            if (event.type == sgal::Event::MouseUp)
                std::cout << "Mouse Up: " << (event.mouse.code == Mouse::Key_LEFT) << "\n";
        }

        window.clear();

        window.update();

        float fps = 1.f / clock.getElapsed();
        clock.restart();
    }
}