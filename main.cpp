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

// Globais
float globalXOffset = 0.0f;
float globalYOffset = 0.0f;
bool mouseAtivo = false;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!mouseAtivo) return;

    static float lastX = 512.0f;
    static float lastY = 384.0f;
    static bool firstMouse = true;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.005f;

    globalXOffset = xoffset * sensitivity;
    globalYOffset = yoffset * sensitivity;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        std::cout << "Botão esquerdo clicado!" << std::endl;

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        std::cout << "X: " << xpos << " | Y: " << ypos << std::endl;
    }
}

int main() {
    // Cria janela e inicializa OpenGL
    Application app(1024, 768, "Cubos OOP");
    if (!app.init()) return -1;

    glfwSetCursorPosCallback(app.getWindow(), mouse_callback);
    glfwSetMouseButtonCallback(app.getWindow(), mouse_button_callback);

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
    pecas.rotation = glm::vec3(0.0f, 1.0f, 0.0f);
    pecas.angle = 0.0f;
    pecas.scale = glm::vec3(1.0f);

    // Instancia o tabuleiro (as peças já são criadas e gerenciadas dentro dele)
    Board tabuleiro(glm::vec3(0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    tabuleiro.setupInitialPieces();

    // Ativa depth test
    glEnable(GL_DEPTH_TEST);

    glm::vec3 cameraPos = glm::vec3(0.0f, 7.0f, 12.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::mat4 view;

    // Velocidade de movimento
    float speedFP = 4.0f;          // WASD primeira pessoa
    float cameraSpeed = 15.0f;     // Câmera geral
    float t = 0.0f;                // Fator da curva de transição
    float transitionDuration = 2.0f; // Duração em segundos
    bool emPrimeiraPessoa = false;
    bool estaTransicionando = false;
    bool pKeyPressionadaUltimoFrame = false;
    bool mKeyPressionadaUltimoFrame = false;
    bool iKeyPressionadaUltimoFrame = false;
    Object* obj_Selected = nullptr;

    glm::vec3 cameraPosInicial = glm::vec3(0.0f);
    glm::vec3 cameraPosDestino = glm::vec3(0.0f);
    glm::vec3 cameraTargetInicial = glm::vec3(0.0f);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    // Loop principal
    while (!glfwWindowShouldClose(app.getWindow())) {
        // Processa input para fechar
        if (glfwGetKey(app.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(app.getWindow(), true);

        // Limpa tela e depth buffer
        glClearColor(0.3f, 0.3f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        shader.setMat4("model", model);

        // Configura projection
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                                800.0f / 600.0f,
                                                0.1f, 100.0f);

        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Input da tecla 'I' para reiniciar as peças do tabuleiro
        bool iKeyTriggered = (glfwGetKey(app.getWindow(), GLFW_KEY_I) == GLFW_PRESS);
        if (iKeyTriggered && !iKeyPressionadaUltimoFrame) {
            tabuleiro.setupInitialPieces();
        }
        iKeyPressionadaUltimoFrame = iKeyTriggered;

        // Input da tecla 'P' para alternar o modo de câmera e iniciar a transição
        bool pKeyTriggered = (glfwGetKey(app.getWindow(), GLFW_KEY_P) == GLFW_PRESS);
        if (pKeyTriggered && !pKeyPressionadaUltimoFrame && !estaTransicionando) {
            emPrimeiraPessoa = !emPrimeiraPessoa;
            estaTransicionando = true;
            t = 0.0f; // Reseta o tempo da curva

            if (emPrimeiraPessoa) {
                cameraPosInicial = cameraPos;
                cameraPosDestino = glm::vec3(-5.0f, 0.8f, 0.0f);
                cameraTargetInicial = cameraTarget;
            } else {
                cameraPosInicial = cameraPos;
                cameraPosDestino = glm::vec3(0.0f, 7.0f, 12.0f);
                cameraTargetInicial = cameraTarget;
            }

            if (mouseAtivo) {
                mouseAtivo = false;
                glfwSetInputMode(app.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        pKeyPressionadaUltimoFrame = pKeyTriggered;

        // Atualização da transição suave da câmera
        if (estaTransicionando) {
            t += deltaTime / transitionDuration;
            if (t >= 1.0f) {
                t = 1.0f;
                estaTransicionando = false;
            }

            float fatorCurva = glm::smoothstep(0.0f, 1.0f, t);

            if (emPrimeiraPessoa) {
                cameraPos = glm::mix(cameraPosInicial, cameraPosDestino, fatorCurva);
            } else {
                cameraPos = glm::mix(cameraPosInicial, cameraPosDestino, fatorCurva);
                cameraTarget = glm::mix(cameraTargetInicial, glm::vec3(0.0f, 0.0f, 0.0f), fatorCurva);
            }
        }

        // Input da tecla 'M' para ativar/desativar o mouse livre
        bool mKeyTriggered = (glfwGetKey(app.getWindow(), GLFW_KEY_M) == GLFW_PRESS);
        if (mKeyTriggered && !mKeyPressionadaUltimoFrame) {
            mouseAtivo = !mouseAtivo;
            if (mouseAtivo) {
                glfwSetInputMode(app.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else {
                glfwSetInputMode(app.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
        }
        mKeyPressionadaUltimoFrame = mKeyTriggered;

        // Movimentação em Primeira Pessoa
        if (emPrimeiraPessoa && !estaTransicionando) {
            glm::vec3 direction = glm::normalize(cameraTarget - cameraPos);
            glm::vec3 right = glm::normalize(glm::cross(direction, cameraUp));

            if (mouseAtivo && (globalXOffset != 0.0f || globalYOffset != 0.0f)) {
                glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), -globalXOffset, glm::vec3(0.0f, 1.0f, 0.0f));
                direction = glm::vec3(rotX * glm::vec4(direction, 0.0f));

                glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), globalYOffset, right);
                direction = glm::vec3(rotY * glm::vec4(direction, 0.0f));

                direction = glm::normalize(direction);

                globalXOffset = 0.0f;
                globalYOffset = 0.0f;
            }

            glm::vec3 moveDirXZ = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));

            if (glfwGetKey(app.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
                cameraPos += moveDirXZ * speedFP * deltaTime;
            if (glfwGetKey(app.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
                cameraPos -= moveDirXZ * speedFP * deltaTime;
            if (glfwGetKey(app.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
                cameraPos -= right * speedFP * deltaTime;
            if (glfwGetKey(app.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
                cameraPos += right * speedFP * deltaTime;

            cameraPos.y = 0.8f;
            cameraTarget = cameraPos + direction;
        }
        // Movimentação Geral / Órbita
        else if (!estaTransicionando) {
            glm::vec3 direction = glm::normalize(cameraTarget - cameraPos);
            glm::vec3 right = glm::normalize(glm::cross(direction, cameraUp));

            if (glfwGetKey(app.getWindow(), GLFW_KEY_A) == GLFW_PRESS)
                cameraPos -= right * cameraSpeed * deltaTime;
            if (glfwGetKey(app.getWindow(), GLFW_KEY_D) == GLFW_PRESS)
                cameraPos += right * cameraSpeed * deltaTime;
            if (glfwGetKey(app.getWindow(), GLFW_KEY_W) == GLFW_PRESS)
                cameraPos.y += cameraSpeed * deltaTime;
            if (glfwGetKey(app.getWindow(), GLFW_KEY_S) == GLFW_PRESS)
                cameraPos.y -= cameraSpeed * deltaTime;
            if (glfwGetKey(app.getWindow(), GLFW_KEY_UP) == GLFW_PRESS)
                cameraPos += direction * cameraSpeed * deltaTime;
            if (glfwGetKey(app.getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS)
                cameraPos -= direction * cameraSpeed * deltaTime;
        }

        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);

        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        // Bind texturas e desenho dos objetos
        tex1.bind(0);
        pecas.draw(shader, model);
        tabuleiro.draw(shader, model);

        // Swap buffers e eventos
        glfwSwapBuffers(app.getWindow());
        glfwPollEvents();
    }

    return 0;
}
