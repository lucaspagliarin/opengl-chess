#ifndef MOVIMENTS_H
#define MOVIMENTS_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Moviments
{
    public:
    Moviments();

    // Atualiza a lógica de câmera, transições e inputs a cada frame
    void update(GLFWwindow* window, float deltaTime);

    // Retorna a matriz de visualização atualizada para o shader
    glm::mat4 getViewMatrix();
    // Getters úteis
    bool isInFirstPerson() const { return emPrimeiraPessoa; }
    bool isTransitioning() const { return estaTransicionando; }

    // Variáveis globais de controle de offset do mouse (chamadas pelo callback)
    static float globalXOffset;
    static float globalYOffset;
    static bool mouseAtivo;

private:
    // Posições e Vetores da Câmera
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;

    // Estados de Câmera
    bool emPrimeiraPessoa;
    bool estaTransicionando;
    bool pKeyPressionadaUltimoFrame;
    bool mKeyPressionadaUltimoFrame;

    // Transição (Smoothstep)
    float t;
    float transitionDuration;
    glm::vec3 cameraPosInicial;
    glm::vec3 cameraPosDestino;
    glm::vec3 cameraTargetInicial;
    glm::vec3 cameraTargetDestino;

    // Velocidades
    float speedFP;
    float cameraSpeed;
};

#endif // MOVIMENTS_H
