#ifndef INCLUDED_LEVEL_LOADER_H
#define INCLUDED_LEVEL_LOADER_H

#include <SDL2/SDL_render.h>

#include <memory>
#include <sol/state.hpp>

#include "../asset_store/asset_store.h"
#include "../ecs/ecs.h"

class LevelLoader {
   public:
    LevelLoader();
    ~LevelLoader();

    void load_level(sol::state& lua, const std::unique_ptr<Registry>& registry,
                    const std::unique_ptr<AssetStore>& asset_store,
                    SDL_Renderer* renderer, int level);
};

#endif
