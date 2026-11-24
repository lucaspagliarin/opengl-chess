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

    // Atualiza a l�gica de c�mera, transi��es e inputs a cada frame
    void update(GLFWwindow* window, float deltaTime);

    // Retorna a matriz de visualiza��o atualizada para o shader
    glm::mat4 getViewMatrix();
    // Getters �teis
    bool isInFirstPerson() const { return emPrimeiraPessoa; }
    bool isTransitioning() const { return estaTransicionando; }
    // Posi��o atual da c�mera no mundo (usada como viewPos na luz especular)
    glm::vec3 getPosition() const { return cameraPos; }

    // Vari�veis globais de controle de offset do mouse (chamadas pelo callback)
    static float globalXOffset;
    static float globalYOffset;
    static bool mouseAtivo;

private:
    // Posi��es e Vetores da C�mera
    glm::vec3 cameraPos;
    glm::vec3 cameraTarget;
    glm::vec3 cameraUp;

    // Estados de C�mera
    bool emPrimeiraPessoa;
    bool estaTransicionando;
    bool pKeyPressionadaUltimoFrame;
    bool mKeyPressionadaUltimoFrame;

    // Transi��o (Smoothstep)
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
