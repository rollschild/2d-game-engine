#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <glm/glm.hpp>
#include <imgui.h>
#include <iostream>
#include <sol/sol.hpp>
#include <sol/types.hpp>

int main() {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);
  std::cout << "hi..." << std::endl;
  return 0;
}
