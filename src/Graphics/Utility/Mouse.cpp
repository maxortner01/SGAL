#include <SGL/SGL.h>

#include <GLFW/glfw3.h>

namespace SGL
{
    static double mouse;
    void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
    {
        mouse += yoffset;
    }

    Vec2f Mouse::getPosition(const Window& window)
    {
        double xpos, ypos;
        glfwGetCursorPos((GLFWwindow*)window.handle, &xpos, &ypos);
        return Vec2f(xpos, ypos);
    }

    MouseButtons Mouse::getInput(const Window& window)
    {
        glfwSetScrollCallback((GLFWwindow*)window.handle, scroll_callback);

        int state_left = glfwGetMouseButton((GLFWwindow*)window.handle, GLFW_MOUSE_BUTTON_LEFT);
        int state_right = glfwGetMouseButton((GLFWwindow*)window.handle, GLFW_MOUSE_BUTTON_LEFT);
        int state_middle = glfwGetMouseButton((GLFWwindow*)window.handle, GLFW_MOUSE_BUTTON_MIDDLE);

        return { state_left == GLFW_PRESS, state_right == GLFW_PRESS, state_middle == GLFW_PRESS, mouse };
    }
}