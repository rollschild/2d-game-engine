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

#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <iostream>

#include "../ecs/ecs.h"
#include "../logger/logger.h"

Game::Game() : is_running(false) { Logger::log("Game constructor called!"); }

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
    window_width = 800;
    window_height = 600;
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
                break;
        }
    }
}

// glm::vec2 player_position;
// glm::vec2 player_velocity;

void Game::setup() {
    // TODO
    // Entity tank = registry.create_entity();
    // tank.add_component<TransformComponent>();
    // tank.add_component<TransformComponent>();
    // player_position = glm::vec2(100.0, 200.0);
    // player_velocity = glm::vec2(50.0, 25.0);
}

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
    // double delta_time = (SDL_GetTicks() - millisecs_previous_frame) / 1000.0;
    // millisecs_previous_frame = SDL_GetTicks();
    // TODO
    //  MovementSystem.update();
    //  CollisionSystem.update();
    //  DamageSystem.update();
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
