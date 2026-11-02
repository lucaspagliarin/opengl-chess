#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>  // se usar glm::rotate, glm::translate etc.
#include <glm/gtc/type_ptr.hpp>

Object::Object()
    : position(0.0f), rotation(1.0f, 0.0f, 0.0f), scale(1.0f), angle(0.0f)
{}

Object::Object(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, float angle)
    : position(pos), rotation(rot), scale(scl), angle(angle)
{
    // construtor básico — inicializa os valores recebidos
}

void Object::draw(Shader& shader, glm::mat4 parentTransform)
{
    // Implementação base vazia — serve para ser sobrescrita nas classes filhas
    // (Cube, Banrisul, etc.)

    // Exemplo: se quiser permitir que as classes derivadas chamem o comportamento padrão:
    // glm::mat4 model = parentTransform;
    // model = glm::translate(model, position);
    // model = glm::rotate(model, glm::radians(angle), rotation);
    // model = glm::scale(model, scale);
    // shader.setMat4("model", model);
}
