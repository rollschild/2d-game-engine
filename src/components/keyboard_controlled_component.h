#ifndef INCLUDED_KEYBOARD_CONTROLLED_COMPONENT_H
#define INCLUDED_KEYBOARD_CONTROLLED_COMPONENT_H

#include <glm/ext/vector_float2.hpp>

struct KeyboardControlledComponent {
    glm::vec2 up_vel;
    glm::vec2 right_vel;
    glm::vec2 down_vel;
    glm::vec2 left_vel;

    KeyboardControlledComponent(glm::vec2 up_vel = glm::vec2(0),
                                glm::vec2 right_vel = glm::vec2(0),
                                glm::vec2 down_vel = glm::vec2(0),
                                glm::vec2 left_vel = glm::vec2(0))
        : up_vel(up_vel),
          right_vel(right_vel),
          down_vel(down_vel),
          left_vel(left_vel) {}
};

#endif
