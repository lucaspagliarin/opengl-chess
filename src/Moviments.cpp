#include "Moviments.h"
#include <Application.h>
#include <Texture.h>
#include <Board.h>
#include <iostream>

float Moviments::globalXOffset = 0.0f;
float Moviments::globalYOffset = 0.0f;
bool Moviments::mouseAtivo = false;

Moviments::Moviments()
    : cameraPos(glm::vec3(0.0f, 7.0f, 12.0f)),
      cameraTarget(glm::vec3(0.0f, 0.0f, 0.0f)),
      cameraUp(glm::vec3(0.0f, 1.0f, 0.0f)),
      emPrimeiraPessoa(false),
      estaTransicionando(false),
      pKeyPressionadaUltimoFrame(false),
      mKeyPressionadaUltimoFrame(false),
      t(0.0f),
      transitionDuration(2.0f),
      speedFP(4.0f),
      cameraSpeed(15.0f),
      cameraPosInicial(0.0f),
      cameraPosDestino(0.0f),
      cameraTargetInicial(0.0f),
      cameraTargetDestino(0.0f) {}

void Moviments::update(GLFWwindow* window, float deltaTime) {
    //tecla p para transição
    bool pKeyTriggered = (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS);
    if (pKeyTriggered && !pKeyPressionadaUltimoFrame && !estaTransicionando) {
        emPrimeiraPessoa = !emPrimeiraPessoa;
        estaTransicionando = true;
        t = 0.0f;

        cameraPosInicial = cameraPos;
        cameraTargetInicial = cameraTarget;

        if (emPrimeiraPessoa) {
            cameraPosDestino = glm::vec3(-5.0f, 1.2f, 0.0f);
            cameraTargetDestino = glm::vec3(0.0f, 0.0f, 0.0f);
        } else {
            cameraPosDestino = glm::vec3(0.0f, 7.0f, 12.0f);
            cameraTargetDestino = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        if (mouseAtivo) {
            mouseAtivo = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    pKeyPressionadaUltimoFrame = pKeyTriggered;
//calculo do fator de curva e atualização da camera
    if (estaTransicionando) {
        t += deltaTime / transitionDuration;
        if (t >= 1.0f) {
            t = 1.0f;
            estaTransicionando = false;
        }
        if(emPrimeiraPessoa)
        {
            mouseAtivo=true;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        float fatorCurva = glm::smoothstep(0.0f, 1.0f, t);
        cameraPos = glm::mix(cameraPosInicial, cameraPosDestino, fatorCurva);
        cameraTarget = glm::mix(cameraTargetInicial, cameraTargetDestino, fatorCurva);
    }
//ativar e desativar mouse para movimentar a camera
    bool mKeyTriggered = (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS);
    if (mKeyTriggered && !mKeyPressionadaUltimoFrame) {
        mouseAtivo = !mouseAtivo;
        if (mouseAtivo) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        } else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
    mKeyPressionadaUltimoFrame = mKeyTriggered;

    if (emPrimeiraPessoa && !estaTransicionando) {
        glm::vec3 direction = glm::normalize(cameraTarget - cameraPos);
        glm::vec3 right = glm::normalize(glm::cross(direction, cameraUp));
//utiliza o offset da movimentação do mouse para rotacionar a camera
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
//movimentação eixo XZ
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos += moveDirXZ * speedFP * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos -= moveDirXZ * speedFP * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= right * speedFP * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += right * speedFP * deltaTime;

        cameraPos.y = 1.2f;
        cameraTarget = cameraPos + direction;

    }
    else if (!estaTransicionando) {
            //movimentação camera sem ser primeira pessoa
        glm::vec3 direction = glm::normalize(cameraTarget - cameraPos);
        glm::vec3 right = glm::normalize(glm::cross(direction, cameraUp));

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            cameraPos -= right * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            cameraPos += right * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            cameraPos.y += cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            cameraPos.y -= cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            cameraPos += direction * cameraSpeed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            cameraPos -= direction * cameraSpeed * deltaTime;
    }
}
//retorna lookat
glm::mat4 Moviments::getViewMatrix() {
    return glm::lookAt(cameraPos, cameraTarget, cameraUp);
}
