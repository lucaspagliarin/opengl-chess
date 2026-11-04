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
#include <Texture.h>
#include <Board.h>
#include <iostream>

int main() {
    // Cria janela e inicializa OpenGL
    Application app(1024, 768, "Cubos OOP");
    if (!app.init()) return -1;

    // Shader
    Shader shader("vertex.glsl", "fragment.glsl");
    shader.use();

    // Carrega texturas
    Texture tex1("wood.jpg");

    shader.setInt("texture1", 0);


    Group pecas(
        glm::vec3(0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f),
        0.0f
    );

    pecas.position = glm::vec3(0.0f, 0.0f, 0.0f);
    pecas.rotation = glm::vec3(0.0f, 1.0f,0.0f);
    pecas.angle = 0.0f;
    pecas.scale = glm::vec3(1.0f);

    // Cores das equipes
    glm::vec4 whiteColor(0.9f, 0.9f, 0.9f, 1.0f);
    glm::vec4 blackColor(0.16f, 0.16f, 0.16f, 1.0f);

    std::vector<Object*> whitePieces;
    std::vector<Object*> blackPieces;

    Pawn* wPawns[8];
    for(int i = 0; i < 8; i++) {
        wPawns[i] = new Pawn(glm::vec3(0.0f), whiteColor);
    }

    Bishop wbispo1(
        glm::vec3(0.0f),
        whiteColor
    );
    Bishop wbispo2(
        glm::vec3(0.0f),
        whiteColor
    );

    Rook wtorre1(
        glm::vec3(0.0f),
        whiteColor
    );
    Rook wtorre2(
        glm::vec3(0.0f),
        whiteColor
    );

    Queen wqueen(
        glm::vec3(0.0f),
        whiteColor
    );

    King wking(
        glm::vec3(0.0f),
        whiteColor
    );

    Knight wknight1(
        glm::vec3(0.0f),
        whiteColor
    );
    Knight wknight2(
        glm::vec3(0.0f),
        whiteColor
    );

    whitePieces.push_back(&wtorre1);
    whitePieces.push_back(&wknight1);
    whitePieces.push_back(&wbispo1);
    whitePieces.push_back(&wqueen);
    whitePieces.push_back(&wking);
    whitePieces.push_back(&wbispo2);
    whitePieces.push_back(&wknight2);
    whitePieces.push_back(&wtorre2);
    for(int i=0;i<8;i++){
        whitePieces.push_back(wPawns[i]);
    }

Pawn* bPawns[8];
    for(int i = 0; i < 8; i++) {
        bPawns[i] = new Pawn(glm::vec3(0.0f), blackColor);
    }

    Bishop bbispo1(
        glm::vec3(0.0f),
        blackColor
    );
    Bishop bbispo2(
        glm::vec3(0.0f),
        blackColor
    );

    Rook btorre1(
        glm::vec3(0.0f),
        blackColor
    );
    Rook btorre2(
        glm::vec3(0.0f),
        blackColor
    );

    Queen bqueen(
        glm::vec3(0.0f),
        blackColor
    );

    King bking(
        glm::vec3(0.0f),
        blackColor
    );

    Knight bknight1(
        glm::vec3(0.0f),
        blackColor
    );
    Knight bknight2(
        glm::vec3(0.0f),
        blackColor
    );

    blackPieces.push_back(&btorre1);
    blackPieces.push_back(&bknight1);
    blackPieces.push_back(&bbispo1);
    blackPieces.push_back(&bqueen);
    blackPieces.push_back(&bking);
    blackPieces.push_back(&bbispo2);
    blackPieces.push_back(&bknight2);
    blackPieces.push_back(&btorre2);
    for(int i=0;i<8;i++){
        blackPieces.push_back(bPawns[i]);
    }

    Board tabuleiro(glm::vec3(0.0f),glm::vec4(1.0f,0.0f,0.0f,1.0f));

    tabuleiro.setupInitialPieces(whitePieces,blackPieces);
    // Ativa depth test
    glEnable(GL_DEPTH_TEST);

    glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 12.0f);

// Para onde a câmera está olhando fixamente (ex: o centro do tabuleiro em {0, 0, 0})
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

// Vetor "para cima" da câmera
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Velocidade de movimento da câmera
float cameraSpeed = 0.1f;
int linha = 6;
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
        //model = glm::rotate(model, (angle * (float) glfwGetTime()) / 20, glm::vec3(0.0f, 0.3f, 0.0f));
        //model = glm::rotate(model, -60.0f, glm::vec3(0.0f, 0.3f, 0.0f));

        shader.setMat4("model", model);

        // Configura view e projection
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                800.0f / 600.0f,
                                                0.1f, 100.0f);
        float deltaTime = 0.05f; // ou calcule o deltaTime real do GLFW se já tiver

        // Movimento lateral (Esquerda / Direita com A e D)
        // Calculamos o vetor "Right" cruzando a direção da visão com o Up
        glm::vec3 direction = glm::normalize(cameraTarget - cameraPos);
        glm::vec3 right = glm::normalize(glm::cross(direction, cameraUp));

        if (glfwGetKey(app.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= right * cameraSpeed * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += right * cameraSpeed * deltaTime;

        // Movimento vertical (Cima / Baixo com W e S no eixo Y do mundo ou relativo)
        // Se quiser subir/descer reto no eixo Y:
        if (glfwGetKey(app.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
            cameraPos.y += cameraSpeed * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
            cameraPos.y -= cameraSpeed * deltaTime;

        // Opcional: Zoom aproximar / afastar (com as setas Cima / Baixo por exemplo)
        if (glfwGetKey(app.getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
            cameraPos += direction * cameraSpeed * deltaTime;
        if (glfwGetKey(app.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
            cameraPos -= direction * cameraSpeed * deltaTime;
            glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);
        //teste movimento
        if (glfwGetKey(app.getWindow(), GLFW_KEY_1) == GLFW_PRESS)
        {
            linha=5;
            tabuleiro.setPieceAt(linha,0,wPawns[0]);
        }
        if (glfwGetKey(app.getWindow(), GLFW_KEY_2) == GLFW_PRESS)
        {
            linha=4;
            tabuleiro.setPieceAt(linha,0,wPawns[0]);
        }

        // Bind texturas
        tex1.bind(0);
        //tex2.bind(1);
        pecas.draw(shader, model);
        tabuleiro.draw(shader,model);
        // Swap buffers e eventos
        glfwSwapBuffers(app.getWindow());
        glfwPollEvents();
    }

    return 0;
}
