#ifndef INCLUDED_ASSET_STORE_H
#define INCLUDED_ASSET_STORE_H

#include <SDL2/SDL_render.h>

#include <map>
#include <string>

class AssetStore {
   private:
    std::map<std::string, SDL_Texture*> textures;

   public:
    AssetStore();
    ~AssetStore();

    void clear_assets();
    void add_texture(SDL_Renderer* renderer, const std::string& asset_id,
                     const std::string& file_path);
    SDL_Texture* get_texture(const std::string& asset_id) const;
};

#endif
