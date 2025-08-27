#include "game.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>

#include <cstdlib>
#include <fstream>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "../components/animation_component.h"
#include "../components/box_collider_component.h"
#include "../components/camera_follow_component.h"
#include "../components/health_component.h"
#include "../components/keyboard_controlled_component.h"
#include "../components/projectile_emitter_component.h"
#include "../components/rigid_body_component.h"
#include "../components/sprite_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"
#include "../logger/logger.h"
#include "../systems/animation_system.h"
#include "../systems/camera_movement_system.h"
#include "../systems/collision_system.h"
#include "../systems/damage_system.h"
#include "../systems/keyboard_control_system.h"
#include "../systems/movement_system.h"
#include "../systems/projectile_emit_system.h"
#include "../systems/projectile_lifecycle_system.h"
#include "../systems/render_collider_system.h"
#include "../systems/render_system.h"

int Game::map_width;
int Game::map_height;
int Game::window_width;
int Game::window_height;

Game::Game() : is_running(false), is_debug(false) {
    registry = std::make_unique<Registry>();
    asset_store = std::make_unique<AssetStore>();
    event_bus = std::make_unique<EventBus>();
    Logger::log("Game constructor called!");
}

Game::~Game() { Logger::log("Game destructor called!"); }

void Game::initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        Logger::err("ERROR initializing SDL!");
        return;
    }

    // Create a window
    // 800x600
    // window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED,
    // SDL_WINDOWPOS_CENTERED, 800, 600,
    // SDL_WINDOW_BORDERLESS);
    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(0, &display_mode);
    // (pseudo) full screen
    // window_width = display_mode.w;
    // window_height = display_mode.h;
    window_width = 1800;
    window_height = 1200;
    window = SDL_CreateWindow(nullptr, SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, window_width,
                              window_height, SDL_WINDOW_BORDERLESS);
    if (!window) {
        Logger::err("ERROR creating SDL window!");
        return;
    }

    // Create a renderer
    // -1: default
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        Logger::err("ERROR creating SDL renderer!");
        return;
    }

    // change video mode to be real full screen
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

    // Initialize camera view with the entire screen area
    camera.x = 0;
    camera.y = 0;
    camera.w = window_width;
    camera.h = window_height;

    is_running = true;
}

void Game::process_input() {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            case SDL_QUIT:
                is_running = false;
                break;
            case SDL_KEYDOWN:
                if (sdl_event.key.keysym.sym == SDLK_ESCAPE) {
                    is_running = false;
                }
                if (sdl_event.key.keysym.sym == SDLK_d) {
                    is_debug = !is_debug;
                }
                event_bus->emit_event<KeyPressedEvent>(
                    sdl_event.key.keysym.sym);
                break;
        }
    }
}

// glm::vec2 player_position;
// glm::vec2 player_velocity;

void Game::load_level(/*int level*/) {
    registry->add_system<MovementSystem>();
    registry->add_system<RenderSystem>();
    registry->add_system<AnimationSystem>();
    registry->add_system<CollisionSystem>();
    registry->add_system<RenderColliderSystem>();
    registry->add_system<DamageSystem>();
    registry->add_system<KeyboardControlSystem>();
    registry->add_system<CameraMovementSystem>();
    registry->add_system<ProjectileEmitSystem>();
    registry->add_system<ProjectileLifecycleSystem>();

    // Add assets
    asset_store->add_texture(renderer, "tank-image",
                             "./assets/images/tank-tiger-right.png");
    asset_store->add_texture(renderer, "truck-image",
                             "./assets/images/truck-ford-right.png");
    asset_store->add_texture(renderer, "tilemap-image",
                             "./assets/tilemaps/jungle.png");
    asset_store->add_texture(renderer, "chopper-image",
                             "./assets/images/chopper-spritesheet.png");
    asset_store->add_texture(renderer, "radar-image",
                             "./assets/images/radar.png");
    asset_store->add_texture(renderer, "bullet-image",
                             "./assets/images/bullet.png");

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

    map_width = num_cols * tile_size * tile_scale;
    map_height = num_rows * tile_size * tile_scale;

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
    radar.add_component<TransformComponent>(glm::vec2(window_width - 74, 10.0),
                                            glm::vec2(1.0, 1.0), 0.0);
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

    // tank.kill();

    // Entity tank = registry.create_entity();
    // tank.add_component<TransformComponent>();
    // tank.add_component<TransformComponent>();
    // player_position = glm::vec2(100.0, 200.0);
    // player_velocity = glm::vec2(50.0, 25.0);
}

void Game::setup() { load_level(); }

void Game::update() {
    // If too fast, wait some time until we reach MILLISECS_PER_FRAME
    // Not effcient!
    /*
    while (!SDL_TICKS_PASSED(SDL_GetTicks(),
                             millisecs_previous_frame + MILLISECS_PER_FRAME))
        ;
    */
    int time_to_wait =
        MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecs_previous_frame);
    if (time_to_wait > 0 && time_to_wait <= MILLISECS_PER_FRAME) {
        SDL_Delay(time_to_wait);
    }

    // converted to seconds
    double delta_time = (SDL_GetTicks() - millisecs_previous_frame) / 1000.0;
    millisecs_previous_frame = SDL_GetTicks();

    // Reset all event handlers for the current frame
    event_bus->reset();

    // Perform subscription of events for all systems
    // the subscription is _PER FRAME_
    registry->get_system<DamageSystem>().subscribe_to_events(event_bus);
    registry->get_system<KeyboardControlSystem>().subscribe_to_events(
        event_bus);
    registry->get_system<ProjectileEmitSystem>().subscribe_to_events(event_bus);

    // update registry to process the entities that waiting to be
    // created/deleted
    registry->update();

    // Invoke all systems that need to update
    registry->get_system<MovementSystem>().update(delta_time);
    registry->get_system<AnimationSystem>().update();
    registry->get_system<CollisionSystem>().update(event_bus);
    registry->get_system<CameraMovementSystem>().update(camera);
    registry->get_system<ProjectileEmitSystem>().update(registry);
    registry->get_system<ProjectileLifecycleSystem>().update();

    //  player_position.x += player_velocity.x * delta_time;
    //  player_position.y += player_velocity.y * delta_time;
}

void Game::run() {
    setup();
    while (is_running) {
        process_input();
        update();
        render();
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    registry->get_system<RenderSystem>().update(renderer, asset_store, camera);
    if (is_debug) {
        registry->get_system<RenderColliderSystem>().update(renderer, camera);
    }

    // draw a PNG texture
    SDL_Surface *surface = IMG_Load("./assets/images/tank-tiger-right.png");
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // TODO
    // copy texture to the destination renderer
    // SDL_Rect dst_rect = {static_cast<int>(player_position.x),
    // static_cast<int>(player_position.y), 32, 32};
    // SDL_RenderCopy(renderer, texture, nullptr, &dst_rect);
    SDL_DestroyTexture(texture);

    // double buffers
    //   - back
    //   - front
    SDL_RenderPresent(renderer);
}

void Game::destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
