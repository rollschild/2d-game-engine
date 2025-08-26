#ifndef INCLUDED_HEALTH_COMPONENT_H
#define INCLUDED_HEALTH_COMPONENT_H

#include <glm/ext/vector_float2.hpp>

struct HealthComponent {
    int health_pct;

    HealthComponent(int health_pct = 0) : health_pct(health_pct) {}
};

#endif
