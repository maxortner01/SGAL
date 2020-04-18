#include <iostream>
#include <vector>

#include <SGAL/SGAL.h>

struct Vector
{
    float angle, magnitude;
};

int main()
{
    using namespace sgal;

    DrawWindow window({ 1780, 920, "Coolio" });

    Light main_light;
    //main_light.position  = Vec3f(0.25f, 1, -1);
    main_light.position  = Vec3f(1, 0, 0);
    main_light.color     = Color(255, 255, 255);
    main_light.type      = Light::Directional;
    main_light.intensity = 40.f;

    LightArray lights;
    lights.push(main_light);
    
    main_light.color     = Color(255, 0, 0, 255);
    main_light.type      = Light::Point;
    main_light.intensity = 100.f;
    //lights.push(main_light);
    
    main_light.position  = Vec3f(-100, 100, -50.f);
    main_light.color     = Color(0, 0, 255, 255);
    main_light.type      = Light::Point;
    main_light.intensity = 100.f;
    //lights.push(main_light);

    RawModel rawModel;
    rawModel.fromFile("res/models/low poly buildings.obj");
    rawModel.calculateNormals();

    Model model(&rawModel);

    /*
    ModelArray model(&rawModel);
    
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

    model.loadMatrices();  */

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
            {
                /**/ if (event.key.code == Keyboard::Key_ESCAPE)
                    window.close();
                else if (event.key.code == Keyboard::Key_TAB)
                {
                    if (rawModel.getRenderMode() == GL::Triangles)
                        rawModel.setRenderMode(GL::Lines);
                    else
                        rawModel.setRenderMode(GL::Triangles);
                }
            }       
        }

        Vec2i mouse_delta = Mouse::getPosition(window) - Vec2i(window.getSize().x / 2, window.getSize().y / 2);
        camera.addRotation({ (float)mouse_delta.y / 50.f, (float)mouse_delta.x / 50.f, 0 });

        Mouse::setPosition({ (int)(window.getSize().x / 2), (int)(window.getSize().y / 2) }, window);

        float speed = 0.5f;

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

        //lights[1].position = camera.getPosition();

        window.clear();

        window.draw(model, &rc);
        model.drawNormals(window, &rc);

        window.update();

        float fps = 1.f / clock.getElapsed();
        clock.restart();

        window.setTitle("Coolio     FPS: " + std::to_string((int)fps));
        frame++;
    }
}