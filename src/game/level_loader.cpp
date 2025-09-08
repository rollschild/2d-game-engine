#include "level_loader.h"

#include <cstdlib>
#include <fstream>
#include <glm/ext/vector_float2.hpp>
#include <sol/error.hpp>
#include <sol/load_result.hpp>
#include <sol/optional_implementation.hpp>
#include <sol/sol.hpp>
#include <sol/state.hpp>
#include <sol/table.hpp>
#include <sol/types.hpp>
#include <string>

#include "../components/animation_component.h"
#include "../components/box_collider_component.h"
#include "../components/camera_follow_component.h"
#include "../components/health_component.h"
#include "../components/keyboard_controlled_component.h"
#include "../components/projectile_emitter_component.h"
#include "../components/rigid_body_component.h"
#include "../components/script_component.h"
#include "../components/sprite_component.h"
#include "../components/text_label_component.h"
#include "../components/transform_component.h"
#include "game.h"

LevelLoader::LevelLoader() {}

LevelLoader::~LevelLoader() {}

void LevelLoader::load_level(
    sol::state& lua, [[maybe_unused]] const std::unique_ptr<Registry>& registry,
    [[maybe_unused]] const std::unique_ptr<AssetStore>& asset_store,
    [[maybe_unused]] SDL_Renderer* renderer,
    [[maybe_unused]] int level_number) {
    // Only checks syntax of the script
    // does _NOT_ execute the script
    sol::load_result script = lua.load_file(
        "./assets/scripts/level" + std::to_string(level_number) + ".lua");
    if (!script.valid()) {
        // error handling
        sol::error err = script;
        std::string error_message = err.what();
        Logger::err("Error loading the Lua script: " + error_message);
        return;
    }

    // executes the script
    lua.script_file("./assets/scripts/level" + std::to_string(level_number) +
                    ".lua");

    sol::table level = lua["level"];
    // Read level assets
    sol::table assets = level["assets"];
    int i = 0;
    while (true) {
        sol::optional<sol::table> has_asset = assets[i];
        if (has_asset == sol::nullopt) {
            break;
        }
        sol::table asset = assets[i];
        std::string asset_type = asset["type"];
        std::string asset_id = asset["id"];
        if (asset_type == "texture") {
            asset_store->add_texture(renderer, asset_id, asset["file"]);
            Logger::log("Successfully loaded new asset to store, ID: " +
                        asset_id);
        }
        if (asset_type == "font") {
            asset_store->add_font(asset_id, asset["file"], asset["font_size"]);
            Logger::log("Successfully add new font to store, ID: " + asset_id);
        }

        i++;
    }

    // Read level tilemap information
    sol::table map = level["tilemap"];
    std::string map_file_path = map["map_file"];
    std::string map_id = map["texture_asset_id"];
    int map_num_rows = map["num_rows"];
    int map_num_cols = map["num_cols"];
    int tile_size = map["tile_size"];
    double map_scale = map["scale"];
    std::fstream map_file;
    map_file.open(map_file_path);
    for (int y = 0; y < map_num_rows; y++) {
        for (int x = 0; x < map_num_cols; x++) {
            char ch;
            map_file.get(ch);
            int src_rect_y = std::atoi(&ch) * tile_size;
            map_file.get(ch);
            int src_rect_x = std::atoi(&ch) * tile_size;
            map_file.ignore();  // skip comma

            Entity tile = registry->create_entity();
            tile.add_component<TransformComponent>(
                glm::vec2(x * (map_scale * tile_size),
                          y * (tile_size * map_scale)),
                glm::vec2(map_scale, map_scale), 0.0);
            tile.add_component<SpriteComponent>("tilemap-texture", tile_size,
                                                tile_size, 0, false, src_rect_x,
                                                src_rect_y);
        }
    }
    map_file.close();
    Game::map_width = map_num_cols * tile_size * map_scale;
    Game::map_height = map_num_rows * tile_size * map_scale;

    // Read entities and their components
    sol::table entities = level["entities"];
    i = 0;
    while (true) {
        sol::optional<sol::table> has_entity = entities[i];
        if (has_entity == sol::nullopt) {
            break;
        }

        sol::table entity = entities[i];
        Entity new_entity = registry->create_entity();

        // check tag
        sol::optional<std::string> tag = entity["tag"];
        if (tag != sol::nullopt) {
            new_entity.tag(entity["tag"]);
        }

        // check group
        sol::optional<std::string> group = entity["group"];
        if (group != sol::nullopt) {
            new_entity.group(entity["group"]);
        }

        // components
        sol::optional<sol::table> has_components = entity["components"];
        if (has_components != sol::nullopt) {
            // Transform
            sol::optional<sol::table> transform =
                entity["components"]["transform"];
            if (transform != sol::nullopt) {
                new_entity.add_component<TransformComponent>(
                    glm::vec2(
                        entity["components"]["transform"]["position"]["x"],
                        entity["components"]["transform"]["position"]["y"]),
                    glm::vec2(
                        entity["components"]["transform"]["scale"]["x"].get_or(
                            1.0),
                        entity["components"]["transform"]["scale"]["y"].get_or(
                            1.0)),
                    entity["components"]["transform"]["rotation"].get_or(0.0));
            }

            // Rigid body
            sol::optional<sol::table> rigid_body =
                entity["components"]["rigid_body"];
            if (rigid_body != sol::nullopt) {
                new_entity.add_component<RigidBodyComponent>(glm::vec2(
                    entity["components"]["rigid_body"]["velocity"]["x"].get_or(
                        0.0),
                    entity["components"]["rigid_body"]["velocity"]["y"].get_or(
                        0.0)));
            }

            // Sprite
            sol::optional<sol::table> sprite = entity["components"]["sprite"];
            if (sprite != sol::nullopt) {
                new_entity.add_component<SpriteComponent>(
                    entity["components"]["sprite"]["texture_asset_id"],
                    entity["components"]["sprite"]["width"],
                    entity["components"]["sprite"]["height"],
                    entity["components"]["sprite"]["z_index"].get_or(1),
                    entity["components"]["sprite"]["fixed"].get_or(false),
                    entity["components"]["sprite"]["src_rect_x"].get_or(0),
                    entity["components"]["sprite"]["src_rect_y"].get_or(0));
            }

            // Animation
            sol::optional<sol::table> animation =
                entity["components"]["animation"];
            if (animation != sol::nullopt) {
                new_entity.add_component<AnimationComponent>(
                    entity["components"]["animation"]["num_frames"].get_or(1),
                    entity["components"]["animation"]["speed_rate"].get_or(1));
            }

            // Box collider
            sol::optional<sol::table> collider =
                entity["components"]["boxcollider"];
            if (collider != sol::nullopt) {
                new_entity.add_component<BoxColliderComponent>(
                    entity["components"]["boxcollider"]["width"],
                    entity["components"]["boxcollider"]["height"],
                    glm::vec2(entity["components"]["boxcollider"]["offset"]["x"]
                                  .get_or(0),
                              entity["components"]["boxcollider"]["offset"]["y"]
                                  .get_or(0)));
            }

            Logger::log("Added box collider");

            // Health
            sol::optional<sol::table> health = entity["components"]["health"];
            if (health != sol::nullopt) {
                new_entity.add_component<HealthComponent>(static_cast<int>(
                    entity["components"]["health"]["health_pct"].get_or(100)));
            }

            // Projectile Emitter
            sol::optional<sol::table> projectile_emitter =
                entity["components"]["projectile_emitter"];
            if (projectile_emitter != sol::nullopt) {
                new_entity.add_component<ProjectileEmitterComponent>(
                    glm::vec2(entity["components"]["projectile_emitter"]
                                    ["projectile_velocity"]["x"],
                              entity["components"]["projectile_emitter"]
                                    ["projectile_velocity"]["y"]),
                    static_cast<int>(entity["components"]["projectile_emitter"]
                                           ["repeat_freq"]
                                               .get_or(1)) *
                        1000,
                    static_cast<int>(entity["components"]["projectile_emitter"]
                                           ["projectile_duration"]
                                               .get_or(10)) *
                        1000,
                    static_cast<int>(entity["components"]["projectile_emitter"]
                                           ["hit_pct_damage"]
                                               .get_or(10)),
                    entity["components"]["projectile_emitter"]["friendly"]
                        .get_or(false)

                );
            }

            // camera follow
            sol::optional<sol::table> camera_follow =
                entity["components"]["camera_follow"];
            if (camera_follow != sol::nullopt) {
                new_entity.add_component<CameraFollowComponent>();
            }

            Logger::log("Added camera follow");

            // Keyboard control
            sol::optional<sol::table> keyboard_control =
                entity["components"]["keyboard_controller"];
            if (keyboard_control != sol::nullopt) {
                new_entity.add_component<KeyboardControlledComponent>(
                    glm::vec2(entity["components"]["keyboard_controller"]
                                    ["up_vel"]["x"],
                              entity["components"]["keyboard_controller"]
                                    ["up_vel"]["y"]),
                    glm::vec2(entity["components"]["keyboard_controller"]
                                    ["right_vel"]["x"],
                              entity["components"]["keyboard_controller"]
                                    ["right_vel"]["y"]),
                    glm::vec2(entity["components"]["keyboard_controller"]
                                    ["down_vel"]["x"],
                              entity["components"]["keyboard_controller"]
                                    ["down_vel"]["y"]),
                    glm::vec2(entity["components"]["keyboard_controller"]
                                    ["left_vel"]["x"],
                              entity["components"]["keyboard_controller"]
                                    ["left_vel"]["y"]));
            }
            Logger::log("Added keyboard_controller");

            // Script
            sol::optional<sol::table> script =
                entity["components"]["on_update_script"];
            if (script != sol::nullopt) {
                sol::function func =
                    entity["components"]["on_update_script"][0];
                new_entity.add_component<ScriptComponent>(func);
            }
        }

        i++;
    }

    /*
    // Add assets
    asset_store->add_texture(renderer, "tank-image",
                             "./assets/images/tank-tiger-right.png");
    asset_store->add_texture(renderer, "truck-image",
                             "./assets/images/truck-ford-right.png");
    asset_store->add_texture(renderer, "tree-image",
                             "./assets/images/tree.png");
    asset_store->add_texture(renderer, "tilemap-image",
                             "./assets/tilemaps/jungle.png");
    asset_store->add_texture(renderer, "chopper-image",
                             "./assets/images/chopper-spritesheet.png");
    asset_store->add_texture(renderer, "radar-image",
                             "./assets/images/radar.png");
    asset_store->add_texture(renderer, "bullet-image",
                             "./assets/images/bullet.png");
    asset_store->add_font("charriot-font", "./assets/fonts/charriot.ttf", 24);
    asset_store->add_font("pico8-font-5", "./assets/fonts/pico8.ttf", 5);
    asset_store->add_font("pico8-font-10", "./assets/fonts/pico8.ttf", 10);

    // Load tilemap
    int tile_size = 32;
    double tile_scale = 3.0;
    int num_cols = 25;
    int num_rows = 20;
    std::fstream map_file;
    map_file.open("./assets/tilemaps/jungle.map");
    for (int y = 0; y < num_rows; y++) {
        for (int x = 0; x < num_cols; x++) {
            char ch;
            map_file.get(ch);
            int src_rect_y = std::atoi(&ch) * tile_size;
            map_file.get(ch);
            int src_rect_x = std::atoi(&ch) * tile_size;
            map_file.ignore();  // skip comma

            Entity tile = registry->create_entity();
            tile.group("tiles");
            tile.add_component<TransformComponent>(
                glm::vec2(x * (tile_scale * tile_size),
                          y * (tile_size * tile_scale)),
                glm::vec2(tile_scale, tile_scale), 0.0);
            tile.add_component<SpriteComponent>("tilemap-image", tile_size,
                                                tile_size, 0, false, src_rect_x,
                                                src_rect_y);
        }
    }
    map_file.close();

    Game::map_width = num_cols * tile_size * tile_scale;
    Game::map_height = num_rows * tile_size * tile_scale;

    Entity chopper = registry->create_entity();
    chopper.tag("player");
    chopper.add_component<TransformComponent>(glm::vec2(100.0, 100.0),
                                              glm::vec2(1.0, 1.0), 0.0);
    chopper.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    chopper.add_component<SpriteComponent>("chopper-image", 32, 32, 1, false);
    chopper.add_component<AnimationComponent>(2, 10, true);
    chopper.add_component<BoxColliderComponent>(32, 32);
    chopper.add_component<KeyboardControlledComponent>(
        glm::vec2(0, -80), glm::vec2(80, 0), glm::vec2(0, 80),
        glm::vec2(-80, 0));
    chopper.add_component<CameraFollowComponent>();
    chopper.add_component<HealthComponent>(100);
    chopper.add_component<ProjectileEmitterComponent>(glm::vec2(150.0, 150.0),
                                                      0, 10000, 10, true);

    Entity radar = registry->create_entity();
    radar.add_component<TransformComponent>(
        glm::vec2(Game::window_width - 74, 10.0), glm::vec2(1.0, 1.0), 0.0);
    radar.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    radar.add_component<SpriteComponent>("radar-image", 64, 64, 2, true);
    radar.add_component<AnimationComponent>(8, 5, true);

    // Create some entities
    Entity tank = registry->create_entity();
    tank.group("enemies");

    // registry->add_component<TransformComponent>(tank, glm::vec2(10.0, 30.0),
    // glm::vec2(1.0, 1.0), 0.0);
    // registry->add_component<RigidBodyComponent>(tank, glm::vec2(50.0, 10.0));

    tank.add_component<TransformComponent>(glm::vec2(1200.0, 10.0),
                                           glm::vec2(1.0, 1.0), 0.0);
    tank.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
    tank.add_component<SpriteComponent>("tank-image", 32, 32, 2, false);
    tank.add_component<BoxColliderComponent>(32, 32);
    tank.add_component<ProjectileEmitterComponent>(glm::vec2(100.0, 0.0), 5000,
                                                   3000, 10, false);
    tank.add_component<HealthComponent>(100);

    Entity truck = registry->create_entity();
    truck.group("enemies");
    truck.add_component<TransformComponent>(glm::vec2(10.0, 10.0),
                                            glm::vec2(1.0, 1.0), 0.0);
    truck.add_component<RigidBodyComponent>(glm::vec2(0.0, 00.0));
    truck.add_component<SpriteComponent>("truck-image", 32, 32, 1, false);
    truck.add_component<BoxColliderComponent>(32, 32);
    truck.add_component<ProjectileEmitterComponent>(glm::vec2(0.0, 100.0), 2000,
                                                    5000, 10, false);
    truck.add_component<HealthComponent>(100);

    Entity tree_a = registry->create_entity();
    tree_a.group("obstacles");
    tree_a.add_component<TransformComponent>(glm::vec2(600.0, 495.0),
                                             glm::vec2(1.0, 1.0), 0.0);
    tree_a.add_component<SpriteComponent>("tree-image", 16, 32, 2);
    tree_a.add_component<BoxColliderComponent>(16, 32);

    Entity tree_b = registry->create_entity();
    tree_b.group("obstacles");
    tree_b.add_component<TransformComponent>(glm::vec2(400.0, 495.0),
                                             glm::vec2(1.0, 1.0), 0.0);
    tree_b.add_component<SpriteComponent>("tree-image", 16, 32, 2);
    tree_b.add_component<BoxColliderComponent>(16, 32);

    Entity label = registry->create_entity();
    SDL_Color white = {255, 255, 255, 0};
    label.add_component<TextLabelComponent>(
        glm::vec2(Game::window_width / 2 - 40, 100), "CHOPPER 1.0",
        "charriot-font", white, true);
    */
}
