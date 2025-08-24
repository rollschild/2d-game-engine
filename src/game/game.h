#ifndef INCLUDED_GAME_H
#define INCLUDED_GAME_H

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <memory>

#include "../asset_store/asset_store.h"
#include "../ecs/ecs.h"
#include "../event_bus/event_bus.h"

#define FPS 60
#define MILLISECS_PER_FRAME (1000 / FPS)

class Game {
   private:
    int millisecs_previous_frame = 0;

    bool is_running;
    bool is_debug;

    SDL_Window *window;
    SDL_Renderer *renderer;

    std::unique_ptr<Registry> registry;
    std::unique_ptr<AssetStore> asset_store;
    std::unique_ptr<EventBus> event_bus;

   public:
    Game();
    ~Game();

    void initialize();
    void run();
    void load_level(/*int level*/);
    void setup();
    void process_input();
    void update();
    void render();
    void destroy();

    int window_width;
    int window_height;
};

#endif
