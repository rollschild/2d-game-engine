#include "./asset_store.h"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>

#include <string>

#include "../logger/logger.h"

AssetStore::AssetStore() { Logger::log("AssetStore constructor called!"); }

AssetStore::~AssetStore() {
    clear_assets();
    Logger::log("AssetStore destructor called!");
}

void AssetStore::clear_assets() {
    for (auto texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void AssetStore::add_texture(SDL_Renderer* renderer,
                             const std::string& asset_id,
                             const std::string& file_path) {
    SDL_Surface* surface = IMG_Load(file_path.c_str());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    // Add texture to map
    textures.emplace(asset_id, texture);
    Logger::log("New texture added to store with ID " + asset_id);
}

SDL_Texture* AssetStore::get_texture(const std::string& asset_id) const {
    return textures.at(asset_id);
}
