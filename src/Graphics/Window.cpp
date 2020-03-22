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

        assert(glfwInit());

        window = glfwCreateWindow(dimensions.x, dimensions.y, title.c_str(), nullptr, nullptr);

        assert(window);

        handle = window;
        bind();

        assert(glewInit() == GLEW_OK);
    }

    Window::~Window()
    {
        glfwDestroyWindow((GLFWwindow*)handle);
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
        glfwMakeContextCurrent((GLFWwindow*)handle);
    }
}