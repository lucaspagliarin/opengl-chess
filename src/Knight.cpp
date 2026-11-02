

#include "Knight.h"

Knight::Knight(glm::vec3 pos, glm::vec4 color)
    : Group(pos),
      base(
          glm::vec3(0.0f, 0.10f, 0.0f),
          0.32f, 0.32f, 0.20f, 32, color
      ),
      neck(
          glm::vec3(0.0f, 0.43f, 0.0f),
          0.42f, 0.55f, color
      ),
      head(
          glm::vec3(0.0f, 0.76f, 0.0f),
          0.34f, 0.38f, color
      ),
      earLeft(
          glm::vec3(-0.10f, 1.00f, 0.0f),
          0.12f, 0.22f, color
      ),
      earRight(
          glm::vec3(0.10f, 1.00f, 0.0f),
          0.12f, 0.22f, color
      )
{
    // Inclina a cabeça para a frente.
    head.rotation = glm::vec3(0.0f, 0.0f, 1.0f);
    head.angle = 20.0f;

    // Inclina ligeiramente as orelhas.
    earLeft.rotation = glm::vec3(0.0f, 0.0f, 1.0f);
    earLeft.angle = -10.0f;

    earRight.rotation = glm::vec3(0.0f, 0.0f, 1.0f);
    earRight.angle = 10.0f;

    addChild(&base);
    addChild(&neck);
    addChild(&head);
    addChild(&earLeft);
    addChild(&earRight);
}
