#include <iostream>
#include <vector>

#include <SGAL/SGAL.h>

int main()
{
    using namespace sgal;

    DrawWindow window({ 1780, 920, "Coolio" });

    Light main_light;
    main_light.position  = Vec3f(0, 0.5f, 1.f);
    main_light.color     = Color(255, 255, 255, 255);
    main_light.type      = Light::Directional;

    LightArray lights;
    lights.push(main_light);
    
    main_light.color     = Color(255, 0, 0, 255);
    main_light.type      = Light::Point;
    main_light.intensity = 10.f;
    lights.push(main_light);
    
    //std::vector<Vec3f> vertices;
    //for (int i = 0; i < 1000; i++)
    //    vertices.push_back(Vec3f( (float)(rand() % 1000) / 500.f - 1.f, (float)(rand() % 1000) / 500.f - 1.f, (float)(rand() % 1000) / 500.f - 1.f ));

    RawModel rawModel;
    rawModel.fromFile("res/models/B_NtSrCompC_0_001.obj");
    //rawModel.fromFile("res/models/syd.obj");
    //rawModel.loadVertices(&vertices[0], vertices.size());
    //rawModel.calculateNormals(&vertices[0], vertices.size());

    rawModel.setRenderMode(GL::Triangles);

    ModelArray model(&rawModel);
    //model.setScale({ -10.f, 10.f, 10.f });

    
    for (int i = -50; i <= 50; i++)
    {
        Model& md = model.makeModel();
        md.setPosition({ i * 20.f, 0, 0 });
        md.setRotation({ 0, 3.14159f, 0 });
    }
    
    for (int i = -50; i <= 50; i++)
    {
        Model& md = model.makeModel();
        md.setPosition({ i * 20.f, 0, -100.f });
    }

    model.loadMatrices(); 

    //model.setScale({ 20.f, 20.f, 20.f });

    Camera camera(3.14159f / 2.f, window);
    
    RenderContext rc;
    rc.shader = &Shader::Default3D();
    rc.camera = &camera;
    rc.lights = &lights;

    unsigned int frame = 0;

    Timer clock;
    while (window.isOpen())
    {
        Vec3f delta;

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

        Vec2i mouse_delta = Mouse::getPosition(window) - Vec2i(window.getSize().x / 2, window.getSize().y / 2);
        camera.addRotation({ (float)mouse_delta.y / 50.f, (float)mouse_delta.x / 50.f, 0 });

        Mouse::setPosition({ (int)(window.getSize().x / 2), (int)(window.getSize().y / 2) }, window);

        float speed = 0.1f;

        if (Keyboard::isKeyPressed(Keyboard::Key_LEFT))
            camera.addRotation({ 0,  0.008f, 0});
        if (Keyboard::isKeyPressed(Keyboard::Key_RIGHT))
            camera.addRotation({ 0, -0.008f, 0});
        if (Keyboard::isKeyPressed(Keyboard::Key_UP))
            camera.addRotation({  0.008f, 0, 0});
        if (Keyboard::isKeyPressed(Keyboard::Key_DOWN))
            camera.addRotation({ -0.008f, 0, 0});


        if (Keyboard::isKeyPressed(Keyboard::Key_LSHIFT))
            speed *= 2.f;
        if (Keyboard::isKeyPressed(Keyboard::Key_A))
            delta.x -= speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_D))
            delta.x += speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_W))
            delta.z += speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_S))
            delta.z -= speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_SPACE))
            delta.y += speed;
        if (Keyboard::isKeyPressed(Keyboard::Key_LCTRL))
            delta.y -= speed;

        camera.step(delta);

        lights[1].position = camera.getPosition();

        window.clear();

        window.draw(model, &rc);

        window.update();

        float fps = 1.f / clock.getElapsed();
        clock.restart();

        window.setTitle("Coolio     FPS: " + std::to_string((int)fps));
        frame++;
    }
}