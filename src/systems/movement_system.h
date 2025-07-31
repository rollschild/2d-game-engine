#ifndef INCLUDED_MOVEMENT_SYSTEM_H
#define INCLUDED_MOVEMENT_SYSTEM_H

class MovementSystem : public System {
  public:
    MovementSystem() {}

    void update() {
        for (auto entity : GetEntities()) {
        }
    }
};

#endif
