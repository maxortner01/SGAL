#include <SGAL/SGAL.h>
#include <iostream>

using namespace sgal;

int main()
{
    DrawWindow window(VideoSettings(1920, 1080, ""));

    UI::Rectangle rectangle;
    rectangle.setColor(Color(255, 0, 0));
    rectangle.setSize(window.getSize() * 0.5f);

    Shader new_shader;
    new_shader.fromString(Shader::DefaultUI().getSource(Shader::Vertex),   Shader::Vertex);
    new_shader.fromString(Shader::DefaultUI().getSource(Shader::Fragment), Shader::Fragment);
    new_shader.link();

    Timer fpstimer;
    while (window.isOpen())
    {
        Event event;
        while (window.poll(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyDown)
                if (event.key.code == Keyboard::Escape)
                    window.close();
        }

        window.clear();

        window.draw(rectangle, &new_shader);

        window.update();

        window.setTitle("Graphing  |  FPS: " + std::to_string((int)(1.0 / fpstimer.getElapsed())));
        fpstimer.restart();
    }
}