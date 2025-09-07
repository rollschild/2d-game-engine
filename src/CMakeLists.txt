add_executable(main main.cpp)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
# find_package(imgui REQUIRED)
find_package(sol2 REQUIRED)
find_package(X11 REQUIRED)

# Manual Lua setup
find_library(LUA_LIBRARY lua)

target_link_libraries(main PRIVATE SDL2::SDL2 ${LUA_LIBRARY} sol2::sol2 X11)

target_sources(
  main
  PRIVATE main.cpp
          game/game.cpp
          game/level_loader.cpp
          logger/logger.cpp
          ecs/ecs.cpp
          asset_store/asset_store.cpp
          ../libs/imgui/imgui.cpp
          ../libs/imgui/imgui_impl_sdl2.cpp
          ../libs/imgui/imgui_impl_sdlrenderer2.cpp
          ../libs/imgui/imgui_demo.cpp
          ../libs/imgui/imgui_draw.cpp
          ../libs/imgui/imgui_tables.cpp
          ../libs/imgui/imgui_widgets.cpp)
