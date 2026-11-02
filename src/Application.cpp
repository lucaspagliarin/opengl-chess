#include "Application.h"
#include <iostream>

Application::Application(int w, int h, const std::string& t)
    : width(w), height(h), title(t), window(nullptr) {}

Application::~Application() {
    glfwTerminate();
}

bool Application::init() {
    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Erro ao criar janela\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (glewInit() != GLEW_OK) {
        std::cerr << "Erro ao inicializar GLEW\n";
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    return true;
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::setTitle(const std::string& t){
    glfwSetWindowTitle(window, t.c_str());
}

void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}
