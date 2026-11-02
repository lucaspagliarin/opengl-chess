#ifndef APPLICATION_H
#define APPLICATION_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

class Application {
private:
    GLFWwindow* window;
    int width, height;
    std::string title;

public:
    Application(int w, int h, const std::string& t);
    ~Application();

    bool init();       // Inicializa GLFW/GLEW
    void run();        // Loop principal
    GLFWwindow* getWindow() const { return window; }
    void setTitle(const std::string& t);

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

#endif
