#include <iostream>
#include <vector>

#include <SGAL/SGAL.h>

int main()
{
    using namespace sgal;

    DrawWindow window({ 1280, 720, "Coolio" });
    
    std::vector<Vec3f> vertices;
    for (int i = 0; i < 100; i++)
        vertices.push_back(Vec3f( (float)(rand() % 1000) / 500.f - 1.f, (float)(rand() % 1000) / 500.f - 1.f, (float)(rand() % 1000) / 500.f - 1.f ));

    RawModel rawModel;
    rawModel.loadVertices(&vertices[0], vertices.size());
    rawModel.setRenderMode(GL::Points);

    Model model(&rawModel);

    Camera camera(3.14159f / 2.f, window);

    Timer clock;
    while (window.isOpen())
    {
        Event event;
        while (window.poll(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::KeyDown)
                if (event.key.code == Keyboard::Key_ESCAPE)
                    window.close();

            /*
            {
                if (event.type == Event::Resize)
                    std::cout << "resize " << event.size.width << " x " << event.size.height << "\n";

                if (event.type == Event::Moving)
                    std::cout << "move to " << event.position.x << " x " << event.position.y << "\n";

                if (event.type == Event::KeyDown)
                    std::cout << "Key Down: " << (char)event.key.code << "\n";
                
                if (event.type == Event::KeyUp)
                    std::cout << "Key Up: " << (char)event.key.code << "\n";

                if (event.type == Event::MouseDown)
                    std::cout << "Mouse Down: " << event.mouse.code << " at " << event.mouse.x << " x " << event.mouse.y << "\n";
                    
                if (event.type == Event::MouseUp)
                    std::cout << "Mouse Up: " << event.mouse.code << " at " << event.mouse.x << " x " << event.mouse.y << "\n";
            }
            */
        }

        RenderContext rc;
        rc.shader = &Shader::Default3D();
        rc.camera = &camera;

        window.clear();

        model.setPosition({ 0, 0, 1 });
        //model.addRotation({ 0.0001f, 0.0001f, 0 });

        window.draw(model, &rc);

        window.update();

        float fps = 1.f / clock.getElapsed();
        clock.restart();
    }
}