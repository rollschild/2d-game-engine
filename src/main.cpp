#include <iostream>
#include <sol/forward.hpp>
#include <sol/sol.hpp>
#include <sol/table.hpp>
#include <sol/types.hpp>

#include "./game/game.h"
// #include <sol/state.hpp>

int native_cpp_cube_function(int n) { return (n * n * n); }

void test_lua() {
    sol::state lua;
    lua.open_libraries(sol::lib::base);

    // Expose and bind a native C++ function to be used by the Lua script
    // should be _BEFORE_ requiring the lua script_file below
    lua["cube"] = native_cpp_cube_function;

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

    // Invoke a Lua function in C++
    sol::function function_factorial = lua["factorial"];
    int func_result = function_factorial(6);
    std::cout << "Lua script calcuated 6! as " << func_result << std::endl;
}

int main(/* int argc, char *argv[] */) {
    Game game;
    game.initialize();
    game.run();
    game.destroy();

    // test_lua();
    return 0;
}
