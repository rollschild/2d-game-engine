#ifndef INCLUDED_SCRIPT_COMPONENT_H
#define INCLUDED_SCRIPT_COMPONENT_H

#include <sol/forward.hpp>
#include <sol/sol.hpp>
#include <sol/types.hpp>

struct ScriptComponent {
    sol::function func;
    ScriptComponent(sol::function func = sol::lua_nil) : func(func) {}
};

#endif
