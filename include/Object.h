#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include "Shader.h"

class Object {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    float angle;

    Object();
    Object(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, float ang);

    virtual void draw(Shader& shader, glm::mat4 parentTransform);
    virtual ~Object() {}

    void setMovementEnabled(bool enabled);

    bool selectForMovement(float liftHeight = 0.3f, float duration = 0.2f);
    bool moveTo(glm::vec3 destination, float duration = 0.4f);
    bool cancelMovement();

    void updateMovement(float deltaTime);

    bool isAnimating() const;
    bool isSelected() const;

private:
    enum class MovementState {
        Idle,
        Lifting,
        Selected,
        MovingHorizontal,
        Lowering,
        ReturningHorizontal,
        ReturningVertical
    };

    bool movementEnabled = false;

    MovementState movementState = MovementState::Idle;

    glm::vec3 originalPosition;
    glm::vec3 phaseStart;
    glm::vec3 phaseTarget;
    glm::vec3 destinationPosition;

    float phaseElapsed = 0.0f;
    float phaseDuration = 0.2f;
    float liftHeight = 0.8f;

    void startPhase(
        glm::vec3 target,
        MovementState nextState,
        float duration
    );
};

#endif
