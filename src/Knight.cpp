
#include "Knight.h"

Knight::Knight(glm::vec3 pos, glm::vec4 color)
    : Group(pos),

      // Base da peca
      base(
          glm::vec3(0.0f, 0.10f, 0.0f),
          0.32f, 0.32f, 0.20f,
          32, color
      ),

      // Corpo afunilado
      body(
          glm::vec3(0.0f, 0.55f, 0.0f),
          0.22f, 0.1f, 0.70f,
          32, color
      ),

      // Cabeca
      head(
          glm::vec3(0.0f, 0.91f, 0.08f),
          glm::vec3(0.0f, 0.0f, 0.0f),
          glm::vec3(0.24f, 0.22f, 0.40f),
          0.0f,
          color,
          true
      ),

      // Orelhas
      earLeft(
          glm::vec3(-0.08f, 1.08f, -0.04f),
          0.10f,
          0.18f,
          color
      ),

      earRight(
          glm::vec3(0.08f, 1.08f, -0.04f),
          0.10f,
          0.18f,
          color
      )
{
    addChild(&base);
    addChild(&body);
    addChild(&head);
    addChild(&earLeft);
    addChild(&earRight);
}
