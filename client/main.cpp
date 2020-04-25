#include <SGAL/SGAL.h>
#include <iostream>

#include "SimplexNoise.h"
#include "SimplexNoise.cpp"

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

struct Map : sgal::Drawable
{
    Map()
    {
        generate();
    }

    void update()
    {
        for (int i = 0; i < water_points.size(); i++)
            water_points[i] = water_points[i] + (shore_normals[i].second * -0.0001f);

        waterModel.rawModel.loadVertices(&water_points[0], water_points.size());   
    }

    void draw(const sgal::Surface* surface, const sgal::RenderContext* rc = nullptr) const override
    {
        surface->draw(landModel, rc);
        surface->draw(normalModel, rc);
        surface->draw(waterModel, rc);
    }

private:
    SingleModel landModel;
    SingleModel normalModel;
    SingleModel waterModel;

    std::vector<sgal::Vec3f> water_points;

    std::vector<std::pair<sgal::Vec3f, sgal::Vec3f>> shore_normals;

    void generate()
    {
        using namespace sgal;

        SimplexNoise noise;

        const unsigned int resolution = 1000;
        const float        radius     = 1.f;
        const float        step       = 3.14159f * 2.f / (float)resolution;
        std::vector<Vec3f> points;

        for (float angle = 0.f; angle <= 3.14159f * 2.f; angle += step)
        {
            const float noise_radius = radius + noise.fractal(5, cos(angle), sin(angle)) / 10.f + noise.fractal(3, cos(angle) / 2.f, sin(angle) / 2.f) / 2.f;

            const Vec3f point = Vec3f(cos(angle) * noise_radius, sin(angle) * noise_radius, 0.f);
            points.push_back(point);
        }

        landModel.rawModel.loadVertices(&points[0], points.size());
        landModel.rawModel.setColor(Color(255, 255, 255));
        landModel.rawModel.setRenderMode(GL::Lines);


        std::vector<Vec3f> normal_lines(points.size() * 2);

        for (int i = 0; i < points.size(); i++)
        {
            Vec3f normal = cross(points[i], points[(i + 1) % points.size()]);
            Vec3f p2 = points[i] - points[(i + 1) % points.size()];
            Vec3f final_normal = normalize(cross(p2, normal));

            normal_lines.push_back(points[i]);
            normal_lines.push_back(points[i] + final_normal);

            shore_normals.push_back(std::pair<sgal::Vec3f, sgal::Vec3f>(points[i], final_normal));
        }

        normalModel.rawModel.loadVertices(&normal_lines[0], normal_lines.size());
        normalModel.rawModel.setColor(Color(255, 0, 0));
        normalModel.rawModel.setRenderMode(GL::Lines);

        const float distance = 2.f;
        for (int i = 0; i < shore_normals.size(); i++)
        {
            const Vec3f shore_point = shore_normals[i].first;
            const Vec3f normal      = shore_normals[i].second;

            const Vec3f start_point = shore_point + normal * distance;
            water_points.push_back(start_point);
        }
        waterModel.rawModel.setDynamic(true);
        waterModel.rawModel.loadVertices(&water_points[0], water_points.size());
        waterModel.rawModel.setColor(Color(255, 255, 255));
        waterModel.rawModel.setRenderMode(GL::Points);
    }
};

int main()
{
    using namespace sgal;

    DrawWindow window({ 1720, 920, "2D Test" });

    RawModel chestRawModel;
    chestRawModel.fromFile("res/models/chest/Chest.obj");

    Model chestModel(&chestRawModel);
    chestModel.setPosition({ 0, 0, 10 });
    //chestModel.setScale(0.1f, 0.1f, 0.1f);

    LightArray lightArray;
    Light main_light;
    main_light.type     = Light::Directional;
    main_light.position = Vec3f(1.f, 0.25f, -1.f);

    lightArray.push(main_light);

    FPSCamera camera(90.f, window);

    RenderContext rc;
    rc.camera = &camera;
    rc.lights = &lightArray;
    rc.shader = &Shader::Default3D();

    UI::Rectangle rectangle;
    rectangle.setSize({ 100.f, (float)window.getSize().y - 10.f });
    rectangle.setColor(Color(255, 0, 0, 120));
    rectangle.setPosition({ 10.f, 10.f });
    rectangle.setRadius(10.f);

    UI::Rectangle second;
    second.setParent(&rectangle);
    second.setSize({ 10.f, 30.f });
    second.setColor(Color(0, 255, 0, 255));

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

        camera.update(window, 0.25f, 50.f);

        window.clear();

        window.draw(chestModel, &rc);
        chestModel.drawNormals(window, &rc);

        chestModel.addRotation({ 0, 0.001f, 0 });

        window.draw(rectangle);
        window.draw(second);

        window.update();
        index++;
    }
}