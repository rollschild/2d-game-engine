#ifndef INCLUDED_SCRIPT_SYSTEM_H
#define INCLUDED_SCRIPT_SYSTEM_H

#include <sol/state.hpp>

#include "../components/script_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

// void get_entity_position(Entity ent) {}

void set_entity_position(Entity ent, double x, double y) {
    if (ent.has_component<TransformComponent>()) {
        auto& transform = ent.get_component<TransformComponent>();
        transform.position.x = x;
        transform.position.y = y;
    } else {
        Logger::err(
            "Trying to set position of an entity that has no transform "
            "component!");
    }
}

class ScriptSystem : public System {
   public:
    ScriptSystem() { require_component<ScriptComponent>(); }

    void create_lua_bindings(sol::state& lua) {
        // Create the "entity" user type so Lua know what an entity is
        lua.new_usertype<Entity>("entity", "get_id", &Entity::get_id, "destroy",
                                 &Entity::kill, "has_tag", &Entity::has_tag,
                                 "in_group", &Entity::in_group);

        lua.set_function("set_position", set_entity_position);
    }

    void update(double delta_time, int ellapsed_time) {
        for (auto ent : get_system_entities()) {
            const auto script = ent.get_component<ScriptComponent>();
            script.func(ent, delta_time, ellapsed_time);
        }
    }
};

#endif
