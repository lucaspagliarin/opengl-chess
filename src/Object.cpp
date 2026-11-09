#include "Object.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

Object::Object()
    : position(0.0f),
      rotation(1.0f, 0.0f, 0.0f),
      scale(1.0f),
      angle(0.0f)
{}

Object::Object(glm::vec3 pos, glm::vec3 rot, glm::vec3 scl, float angle)
    : position(pos),
      rotation(rot),
      scale(scl),
      angle(angle)
{}

void Object::draw(Shader& shader, glm::mat4 parentTransform)
{
}

void Object::setMovementEnabled(bool enabled)
{
    movementEnabled = enabled;
}

void Object::startPhase(
    glm::vec3 target,
    MovementState nextState,
    float duration
) {
    phaseStart = position;
    phaseTarget = target;
    movementState = nextState;

    phaseElapsed = 0.0f;
    phaseDuration = std::max(duration, 0.001f);
}

bool Object::selectForMovement(float height, float duration)
{
    if (!movementEnabled || movementState != MovementState::Idle)
        return false;

    originalPosition = position;
    liftHeight = height;

    glm::vec3 target = originalPosition;
    target.y += liftHeight;

    startPhase(target, MovementState::Lifting, duration);

    return true;
}

bool Object::moveTo(glm::vec3 destination, float duration)
{
    if (movementState != MovementState::Selected)
        return false;

    destinationPosition = destination;

    // Mantém a peça suspensa durante o deslocamento horizontal.
    destinationPosition.y = originalPosition.y + liftHeight;

    startPhase(
        destinationPosition,
        MovementState::MovingHorizontal,
        duration
    );

    return true;
}

bool Object::cancelMovement()
{
    if (movementState == MovementState::Idle)
        return false;

    if (movementState == MovementState::ReturningHorizontal ||
        movementState == MovementState::ReturningVertical)
        return true;

    // Se a peça ainda está na casa original, basta descer.
    if (movementState == MovementState::Lifting ||
        movementState == MovementState::Selected) {

        startPhase(
            originalPosition,
            MovementState::ReturningVertical,
            0.2f
        );

        return true;
    }

    // Se já começou a se deslocar, primeiro retorna horizontalmente.
    glm::vec3 returnPosition = position;
    returnPosition.x = originalPosition.x;
    returnPosition.z = originalPosition.z;

    startPhase(
        returnPosition,
        MovementState::ReturningHorizontal,
        0.3f
    );

    return true;
}

void Object::updateMovement(float deltaTime)
{
    if (movementState == MovementState::Idle ||
        movementState == MovementState::Selected)
        return;

    phaseElapsed += deltaTime;

    float t = phaseElapsed / phaseDuration;
    t = glm::clamp(t, 0.0f, 1.0f);

    // Smoothstep: acelera suavemente e desacelera no final.
    float easedT = t * t * (3.0f - 2.0f * t);

    position = glm::mix(phaseStart, phaseTarget, easedT);

    if (t < 1.0f)
        return;

    position = phaseTarget;

    switch (movementState) {

        case MovementState::Lifting:
            movementState = MovementState::Selected;
            break;

        case MovementState::MovingHorizontal: {
            glm::vec3 target = destinationPosition;
            target.y = originalPosition.y;

            startPhase(target, MovementState::Lowering, 0.2f);
            break;
        }

        case MovementState::Lowering:
            position = destinationPosition;
            position.y = originalPosition.y;
            movementState = MovementState::Idle;
            break;

        case MovementState::ReturningHorizontal:
            startPhase(
                originalPosition,
                MovementState::ReturningVertical,
                0.2f
            );
            break;

        case MovementState::ReturningVertical:
            position = originalPosition;
            movementState = MovementState::Idle;
            break;

        default:
            break;
    }
}
void Object::placeAtWithAnimation(glm::vec3 destination, float duration) {
    originalPosition = destination;
    liftHeight = 1.5f;

    position = destination;
    position.y += liftHeight; // Começa no alto

    startPhase(destination, MovementState::Lowering, duration);
}

bool Object::isAnimating() const
{
    return movementState != MovementState::Idle &&
           movementState != MovementState::Selected;
}

bool Object::isSelected() const
{
    return movementState != MovementState::Idle;
}


