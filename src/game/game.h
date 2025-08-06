#ifndef INCLUDED_GAME_H
#define INCLUDED_GAME_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <memory>

#include "../ecs/ecs.h"

#define FPS 60
#define MILLISECS_PER_FRAME (1000 / FPS)

class Game {
   private:
    int millisecs_previous_frame = 0;

    bool is_running;

    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<Registry> registry;

   public:
    Game();
    ~Game();

    void initialize();
    void run();
    void setup();
    void process_input();
    void update();
    void render();
    void destroy();

    int window_width;
    int window_height;
};

#endif
