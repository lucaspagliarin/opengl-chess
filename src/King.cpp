

#include "King.h"

King::King(glm::vec3 pos, glm::vec4 color)
    : Group(pos),
      base(
          glm::vec3(0.0f, 0.10f, 0.0f),
          0.34f, 0.34f, 0.20f, 32, color
      ),
      body(
          glm::vec3(0.0f, 0.55f, 0.0f),
          0.24f, 0.14f, 0.70f, 32, color
      ),
      collar(
        glm::vec3(0.0f, 0.90f, 0.0f),
        0.19f, 0.19f,
        0.08f, 32, color
      ),
      head(glm::vec3(0.0f, 1.02f, 0.0f)),
      head_bottom(
        glm::vec3(0.0f, 0.0f, 0.0f),
        0.10f,  // raio inferior
        0.15f, // raio superior
        0.3f,  // altura
        32,
        color
      ),
      head_sphere(
        glm::vec3(0.0f, 0.15f, 0.0f),
        0.13f,
        24,
        12,
        color
      ),
      tip(
        glm::vec3(0.0f, 0.25f, 0.0f),
        0.055f,
        24,
        12,
        color
      ),
      crossVertical(
          glm::vec3(0.0f, 1.38f, 0.0f),
          glm::vec3(0.0f),
          glm::vec3(0.06f, 0.25f, 0.06f),
            glm::degrees(angle),
          color,
          true
      ),
      crossHorizontal(
          glm::vec3(0.0f, 1.41f, 0.0f),
          glm::vec3(0.0f),
          glm::vec3(0.21f, 0.06f, 0.06f),
            glm::degrees(angle),
          color,
          true
      )
{
    head.addChild(&head_bottom);

    head_sphere.scale = glm::vec3(1.0f, 0.5f, 1.0f);

    head.addChild(&head_sphere);
    //head.addChild(&tip);

    head.scale = glm::vec3(1.2f);

    addChild(&base);
    addChild(&body);
    addChild(&collar);
    addChild(&head);


    addChild(&crossVertical);
    addChild(&crossHorizontal);
}
