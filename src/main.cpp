#include <iostream>
#include <sol/sol.hpp>
#include <sol/table.hpp>
#include <sol/types.hpp>

#include "./game/game.h"
// #include <sol/state.hpp>

void test_lua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);
    lua.script_file("./assets/scripts/script.lua");
    int some_var_inside_cpp = lua["some_var"];
    std::cout << "Value from lua script: " << some_var_inside_cpp << std::endl;

    bool is_fullscreen = lua["config"]["fullscreen"];
    sol::table config = lua["config"];
    int width = config["resolution"]["width"];
    int height = config["resolution"]["height"];
    std::cout << "We read the value fullscreen: " << is_fullscreen << std::endl;
    std::cout << "window width: " << width << std::endl;
    std::cout << "window height: " << height << std::endl;
}

int main(/* int argc, char *argv[] */) {
    // Game game;
    // game.initialize();
    // game.run();
    // game.destroy();

    test_lua();
    return 0;
}
