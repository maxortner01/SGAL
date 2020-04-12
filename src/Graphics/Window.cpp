#include <SGL/SGL.h>

#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace SGL
{
    Window::Window(Vec2u dimensions, const std::string& title) :
        Surface(dimensions)
    {
        GLFWwindow* window;

        std::cout << "GLFWinit\n";
        assert(glfwInit());
        std::cout << "done\n";

        std::cout << "creating window\n";
        window = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), nullptr, nullptr);
        std::cout << "done\n";

        assert(window);

        handle = window;
        glfwMakeContextCurrent(window);

        std::cout << "GLEWinit\n";
        assert(glewInit() == GLEW_OK);
        std::cout << "done\n";
    }

    Window::~Window()
    {
        std::cout << "window\n";
        glfwDestroyWindow((GLFWwindow*)handle);
        std::cout << "window1\n";
    }

    void Window::display() const
    {
        glfwSwapBuffers((GLFWwindow*)handle);
        glfwPollEvents();
    }

    bool Window::opened() const
    {
        return (!glfwWindowShouldClose((GLFWwindow*)handle));
    }

    void Window::bind() const
    {
        //glfwMakeContextCurrent((GLFWwindow*)handle);
    }
}