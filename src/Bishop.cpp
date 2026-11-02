
#include "Bishop.h"

Bishop::Bishop(glm::vec3 pos, glm::vec4 color)
    : Group(pos),
      base(
          glm::vec3(0.0f, 0.10f, 0.0f),
          0.32f, 0.32f, 0.20f, 32, color
      ),
      body(
          glm::vec3(0.0f, 0.50f, 0.0f),
          0.22f, 0.12f, 0.60f, 32, color
      ),
      collar(
        glm::vec3(0.0f, 0.78f, 0.0f),
        0.19f, 0.19f,
        0.08f, 32, color
      ),
      head_bottom(
        glm::vec3(0.0f, 0.88f, 0.0f),
        0.10f,  // raio inferior
        0.15f, // raio superior
        0.18f,  // altura
        32,
        color
        ),
      head_top(
        glm::vec3(0.0f, 1.02f, 0.0f),
        0.15f,  // raio inferior
        0.035f, // raio superior
        0.10f,  // altura
        32,
        color
        ),
        tip(
            glm::vec3(0.0f, 1.09f, 0.0f),
            0.055f,
            24,
            12,
            color
        )
{
    addChild(&base);
    addChild(&body);
    addChild(&collar);
    addChild(&head_bottom);
    addChild(&head_top);
    addChild(&tip);
}

