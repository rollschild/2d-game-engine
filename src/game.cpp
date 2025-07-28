#include "game.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <glm/ext/vector_float2.hpp>
#include <glm/glm.hpp>
#include <iostream>

Game::Game() : is_running(false) {
    std::cout << "Game constructor called!" << std::endl;
}

Game::~Game() { std::cout << "Game destructor called!" << std::endl; }

void Game::initialize() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "ERROR initializing SDL!" << std::endl;
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
        std::cerr << "ERROR creating SDL window!" << std::endl;
        return;
    }

    // Create a renderer
    // -1: default
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "ERROR creating SDL renderer!" << std::endl;
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

glm::vec2 player_position;
glm::vec2 player_velocity;

void Game::setup() {
    player_position = glm::vec2(100.0, 200.0);
    player_velocity = glm::vec2(0.050, 0.0);
}

void Game::update() {
    player_position.x += player_velocity.x;
    player_position.y += player_velocity.y;
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

    // copy texture to the destination renderer
    SDL_Rect dst_rect = {static_cast<int>(player_position.x),
                         static_cast<int>(player_position.y), 32, 32};
    SDL_RenderCopy(renderer, texture, nullptr, &dst_rect);
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
