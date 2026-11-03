#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.h>
#include <Application.h>
#include <Cube.h>
#include <Cylinder.h>
#include <Pyramid.h>
#include <Sphere.h>
#include <Group.h>
#include <Pawn.h>
#include <Rook.h>
#include <Queen.h>
#include <King.h>
#include <Knight.h>
#include <Bishop.h>
#include <Banrisul.h>
#include <Texture.h>
#include <iostream>

int main() {
    // Cria janela e inicializa OpenGL
    Application app(1024, 768, "Cubos OOP");
    if (!app.init()) return -1;

    // Shader
    Shader shader("vertex.glsl", "fragment.glsl");
    shader.use();

    // Carrega texturas
    Texture tex1("pedra-28.jpg");
    Texture tex2("opengl.png");  // logo OpenGL com alpha

    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);


    Group teste(
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f),
        0.0f
    );

    teste.position = glm::vec3(0.0f, 0.0f, 0.0f);
    teste.rotation = glm::vec3(0.4f, 1.0f,0.0f);
    teste.angle = 45.0f;
    teste.scale = glm::vec3(1.4f);

    Pawn peao(
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

    Bishop bispo(
        glm::vec3(-1.0f, 0.0f, 1.0f),
        glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

    Rook torre(
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

    Queen queen(
        glm::vec3(2.0f, 0.0f, 1.0f),
        glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

    King king(
        glm::vec3(-2.0f, 0.0f, 1.0f),
        glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

    Knight knight(
        glm::vec3(3.0f, 0.0f, 1.0f),
        glm::vec4(0.9f, 0.9f, 0.9f, 1.0f)
    );

    teste.addChild(&peao);
    teste.addChild(&bispo);
    teste.addChild(&torre);
    teste.addChild(&queen);
    teste.addChild(&king);
    teste.addChild(&knight);

    Pawn peaob(
        glm::vec3(0.0f, 0.0f, -1.0f),
        glm::vec4(0.16f, 0.16f, 0.16f, 1.0f)
    );

    Bishop bispob(
        glm::vec3(-1.0f, 0.0f, -1.0f),
        glm::vec4(0.16f, 0.16f, 0.16f, 1.0f)
    );

    Rook torreb(
        glm::vec3(1.0f, 0.0f, -1.0f),
        glm::vec4(0.16f, 0.16f, 0.16f, 1.0f)
    );

    Queen queenb(
        glm::vec3(2.0f, 0.0f, -1.0f),
        glm::vec4(0.16f, 0.16f, 0.16f, 1.0f)
    );

    King kingb(
        glm::vec3(-2.0f, 0.0f, -1.0f),
        glm::vec4(0.16f, 0.16f, 0.16f, 1.0f)
    );

    Knight knightb(
        glm::vec3(3.0f, 0.0f, -1.0f),
        glm::vec4(0.16f, 0.16f, 0.16f, 1.0f)
    );

    teste.addChild(&peaob);
    teste.addChild(&bispob);
    teste.addChild(&torreb);
    teste.addChild(&queenb);
    teste.addChild(&kingb);
    teste.addChild(&knightb);


    // Ativa depth test
    glEnable(GL_DEPTH_TEST);

    // Loop principal
    while (!glfwWindowShouldClose(app.getWindow())) {
        // Processa input
        if (glfwGetKey(app.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(app.getWindow(), true);

        // Limpa tela e depth buffer
        glClearColor(0.3f, 0.3f, 0.5f, 1.0f);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        float angle = 20.0f;
        model = glm::rotate(model, (angle * (float) glfwGetTime()) / 20, glm::vec3(0.0f, 0.3f, 0.0f));
        //model = glm::rotate(model, -60.0f, glm::vec3(0.0f, 0.3f, 0.0f));

        shader.setMat4("model", model);

        // Configura view e projection
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                800.0f / 600.0f,
                                                0.1f, 100.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.0f, -12.0f));
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Bind texturas
        tex1.bind(0);
        tex2.bind(1);

        teste.draw(shader, model);

        // Swap buffers e eventos
        glfwSwapBuffers(app.getWindow());
        glfwPollEvents();
    }

    return 0;
}
