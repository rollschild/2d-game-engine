#include "ecs.h"

#include <algorithm>
#include <ranges>
#include <vector>

int Entity::get_id() const { return id; }

void System::add_entity_to_system(Entity entity) { entities.push_back(entity); }

void System::remove_entity_from_system(Entity entity) {
    // Old syntax
    /*
    entities.erase(std::remove_if(entities.begin(), entities.end(),
                                  [&entity](Entity other) {
                                      return entity.get_id() == other.get_id();
                                  }),
                   entities.end());
   */

    // since C++20
    std::erase_if(entities,
                  [&entity](Entity other) { return entity == other; });

    // c++ alternative, more performant?
    /*
    entities.erase(std::ranges::find_if(entities, [&entity](Entity other) {
        return entity == other;
    }));
    */
}

std::vector<Entity> System::get_system_entities() const { return entities; }

const Signature& System::get_component_signature() const {
    return component_signature;
}
