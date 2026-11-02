#include "Banrisul.h"
#include "Cube.h"

Banrisul::Banrisul(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, float angle)
    : Object(pos, rot, scl, angle)
{
    // Cada parte da nave será um "sub-modelo"

    init();
}

Banrisul::Banrisul(glm::vec3 pos, float angle)
    : Object(pos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), angle)
{
    init();
}

void Banrisul::init() {

    parts.push_back(std::make_unique<Cube>(
        glm::vec3(0.0f, 0.0f, 0.0f),     // posição
        glm::vec3(1.0f, 0.0f, 0.0f),      // rotação (eixo)
        glm::vec3(1.0f, 1.0f, 1.0f),      // escala
        45.0f   // Angulo
    ));

    parts.push_back(std::make_unique<Cube>(
        glm::vec3(0.33f, 0.66f, -0.33f),     // posição
        glm::vec3(1.0f, 0.0f, 0.0f),      // rotação (eixo)
        glm::vec3(1.0f, 1.0f, 1.0f),      // escala
        45.0f   // Angulo
    ));

    parts.push_back(std::make_unique<Cube>(
        glm::vec3(0.66f, 0.0f, -0.66f),     // posição
        glm::vec3(1.0f, 0.0f, 0.0f),      // rotação (eixo)
        glm::vec3(1.0f, 1.0f, 1.0f),      // escala
        45.0f   // Angulo
    ));
    /*
    parts.push_back(std::make_unique<Cube>(
        glm::vec3(1.66f, 0.0f, 0.0f),     // posição
        glm::vec3(0.0f, 0.0f, 1.0f),      // rotação (eixo)
        glm::vec3(1.0f, 0.1f, 1.0f),      // escala
        30.0f   // Angulo
    ));

        parts.push_back(std::make_unique<Cube>(
        glm::vec3(2.66f, 0.0f, 0.0f),     // posição
        glm::vec3(0.0f, 0.0f, 1.0f),      // rotação (eixo)
        glm::vec3(1.0f, 0.1f, 1.0f),      // escala
        -30.0f   // Angulo
    ));
    */


}

void Banrisul::draw(Shader &shader, glm::mat4 model) {

    model = glm::translate(model, position);
    model = glm::scale(model, scale);

    for (auto &part : parts) {
        part->draw(shader, model);
    }
}

