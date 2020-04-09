#include <iostream>

#include <SGAL/SGAL.h>

int main()
{
    using namespace sgal;

    Window window({ 1280, 720, "Coolio" });

    while (window.isOpen())
    {
        window.Update();
    }
}