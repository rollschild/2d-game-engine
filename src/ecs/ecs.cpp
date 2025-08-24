#include "ecs.h"

#include <algorithm>
#include <ranges>
#include <string>
#include <utility>
#include <vector>

#include "../logger/logger.h"

unsigned IComponent::next_id = 0;

int Entity::get_id() const { return id; }

void Registry::add_entity_to_systems(Entity entity) {
    const auto entity_id = entity.get_id();
    const auto& ent_component_sig = entity_component_sigs[entity_id];

    for (auto& system : systems) {
        const auto& system_component_sig =
            system.second->get_component_signature();
        bool is_interested =
            (ent_component_sig & system_component_sig) == system_component_sig;

        if (is_interested) {
            system.second->add_entity_to_system(entity);
        }
    }
}

void Entity::kill() { registry->kill_entity(*this); }

void Registry::remove_entity_from_systems(Entity entity) {
    for (auto system : systems) {
        system.second->remove_entity_from_system(entity);
    }
}

void System::add_entity_to_system(Entity ent) { entities.push_back(ent); }

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

Entity Registry::create_entity() {
    unsigned entity_id;

    if (free_ids.empty()) {
        entity_id = num_entities++;
        // make sure the entity_component_sigs vector can handle the new entity
        if (entity_id >= entity_component_sigs.size()) {
            entity_component_sigs.resize(entity_id + 1);
        }
    } else {
        entity_id = free_ids.front();
        free_ids.pop_front();
    }

    Entity entity(entity_id);
    entity.registry = this;
    entities_to_be_added.insert(entity);

    Logger::log("Entity created with id = " + std::to_string(entity_id));
    return entity;
}

void Registry::kill_entity(Entity ent) { entities_to_be_killed.insert(ent); }

void Registry::update() {
    // Add entities waiting to be added to the active systems
    for (auto entity : entities_to_be_added) {
        add_entity_to_systems(entity);
    }
    entities_to_be_added.clear();

    for (auto ent : entities_to_be_killed) {
        remove_entity_from_systems(ent);
        entity_component_sigs[ent.get_id()].reset();
        free_ids.push_back(ent.get_id());
    }
    entities_to_be_killed.clear();
}
