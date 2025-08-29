#include "ecs.h"

#include <algorithm>
#include <ranges>
#include <set>
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

void Entity::tag(const std::string& tag) { registry->tag_entity(*this, tag); }

bool Entity::has_tag(const std::string& tag) const {
    return registry->entity_has_tag(*this, tag);
}

void Entity::group(const std::string& group) {
    registry->group_entity(*this, group);
}

bool Entity::in_group(const std::string& group) const {
    return registry->entity_in_group(*this, group);
}

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

        // remove entity from component pools
        for (auto pool : component_pools) {
            if (pool) {
                pool->remove_entity_from_pool(ent.get_id());
            }
        }

        free_ids.push_back(ent.get_id());

        remove_entity_tag(ent);
        remove_entity_group(ent);
    }
    entities_to_be_killed.clear();
}

void Registry::tag_entity(Entity ent, const std::string& tag) {
    entity_tag_map.emplace(ent.get_id(), tag);
    tag_entity_map.emplace(tag, ent);
}

bool Registry::entity_has_tag(Entity ent, const std::string& tag) const {
    if (entity_tag_map.find(ent.get_id()) == entity_tag_map.end()) {
        return false;
    }

    return tag_entity_map.find(tag)->second == ent;
}

void Registry::group_entity(Entity ent, const std::string& group) {
    group_entities_map.emplace(group, std::set<Entity>());
    group_entities_map[group].emplace(ent);
    entity_group_map.emplace(ent.get_id(), group);
}

bool Registry::entity_in_group(Entity ent, const std::string& group) const {
    if (group_entities_map.find(group) == group_entities_map.end()) {
        return false;
    }
    auto entities = group_entities_map.at(group);
    return entities.find(ent.get_id()) != entities.end();
}

Entity Registry::get_entity_by_tag(const std::string& tag) const {
    return tag_entity_map.at(tag);
}

std::vector<Entity> Registry::get_entities_by_group(
    const std::string& group) const {
    auto& set_of_entities = group_entities_map.at(group);
    return std::vector<Entity>(set_of_entities.begin(), set_of_entities.end());
}

void Registry::remove_entity_tag(Entity ent) {
    auto tagged_ent = entity_tag_map.find(ent.get_id());
    if (tagged_ent != entity_tag_map.end()) {
        auto tag = tagged_ent->second;
        tag_entity_map.erase(tag);         // erase by key
        entity_tag_map.erase(tagged_ent);  // erase by iterator
    }
}

void Registry::remove_entity_group(Entity ent) {
    auto grouped_ent = entity_group_map.find(ent.get_id());
    if (grouped_ent != entity_group_map.end()) {
        auto group = group_entities_map.find(grouped_ent->second);
        if (group != group_entities_map.end()) {
            auto entity_in_group = group->second.find(ent);
            if (entity_in_group != group->second.end()) {
                group->second.erase(entity_in_group);
            }
        }
        entity_group_map.erase(grouped_ent);
    }
}
