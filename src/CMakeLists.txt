add_executable(main main.cpp)
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(sol2 REQUIRED)

# Manual Lua setup
find_library(LUA_LIBRARY lua)

target_link_libraries(main PRIVATE SDL2::SDL2 ${LUA_LIBRARY} sol2::sol2
                                   imgui::imgui)

target_sources(main PRIVATE main.cpp game/game.cpp logger/logger.cpp)
