#include <SGAL/SGAL.h>
#include <iostream>

struct SingleModel : sgal::Drawable
{
    SingleModel() :
        model(&rawModel)
    {   }

    void draw(const sgal::Surface* surface, const sgal::RenderContext* rc = nullptr) const override
    {
        surface->draw(model, rc);
    }

    sgal::RawModel rawModel;
    sgal::Model    model;
};

int main()
{
    using namespace sgal;

    DrawWindow window({ 1920, 1080, "2D Test" });

    RawModel chestRawModel;
    chestRawModel.fromFile("res/models/B_NtSrCompC_0_001.obj");

    Model chestModel(&chestRawModel);
    chestModel.setPosition({ 0, 0, 10 });
    chestModel.setScale(10.f, 10.f, 10.f);

    LightArray lightArray;
    Light main_light;
    main_light.type     = Light::Directional;
    main_light.position = Vec3f(1.f, 0.25f, -1.f);

    lightArray.push(main_light);

    FPSCamera camera(3.14159f / 2.f, window);

    RenderContext rc;
    rc.camera = &camera;
    rc.lights = &lightArray;
    rc.shader = &Shader::Default3D();

    UI::Rectangle rectangle;
    rectangle.setSize({ 200.f, (float)window.getSize().y - 20.f });
    rectangle.setColor(Color(255, 0, 0, 255));
    rectangle.setPosition({ 10.f, 10.f });
    rectangle.setRadius(10.f);
    
    DrawTexture test(Vec2u{ 180, 180 });

    Texture texture;
    texture.fromFile("res/textures/test.png");

    UI::Rectangle texture_test;
    texture_test.setParent(&rectangle);
    texture_test.setSize(rectangle.getSize() + Vec2f(-rectangle.getRadius() * 2.f, -rectangle.getRadius() * 2.f));
    texture_test.setSize({ texture_test.getSize().x, texture_test.getSize().x });
    texture_test.setColor(Color(255, 255, 255, 255));
    texture_test.setTexture(test.texture());
    texture_test.setRadius(10.f); 

    unsigned int index = 0;
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
        }

        camera.update(window, 1.f, 50.f);

        window.clear(Color(75, 75, 75));

        camera.setSurface(window);
        window.draw(chestModel, &rc);
        chestModel.drawNormals(window, &rc);

        camera.setSurface(test);
        test.clear(Color(255, 0, 0));
        test.draw(chestModel, &rc);
        chestModel.drawNormals(test, &rc);

        chestModel.addRotation({ 0, 0.01f, 0 });

        window.draw(rectangle);
        window.draw(texture_test);

        window.update();
        index++;
    }
}