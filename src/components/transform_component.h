#ifndef INCLUDED_TRANSFORM_COMPONENT_H
#define INCLUDED_TRANSFORM_COMPONENT_H

#include <glm/glm.hpp>

struct TransformComponent {
    glm::vec2 position;
    glm::vec2 scale;

    double rotation;
};

#endif
