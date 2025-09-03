#ifndef INCLUDED_RENDER_GUI_SYSTEM_H
#define INCLUDED_RENDER_GUI_SYSTEM_H

#include <SDL2/SDL_render.h>
#include <libs/imgui/imgui.h>
#include <libs/imgui/imgui_impl_sdl2.h>
#include <libs/imgui/imgui_impl_sdlrenderer2.h>

#include <glm/ext/vector_float2.hpp>
#include <memory>

#include "../components/box_collider_component.h"
#include "../components/health_component.h"
#include "../components/projectile_emitter_component.h"
#include "../components/rigid_body_component.h"
#include "../components/sprite_component.h"
#include "../components/transform_component.h"
#include "../ecs/ecs.h"

class RenderGUISystem : public System {
   public:
    RenderGUISystem() = default;

    void update(SDL_Renderer* renderer,
                const std::unique_ptr<Registry>& registry) {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Spawn enemies")) {
            // ImGui::Text("Here is where we spawn new enemies...");
            static int enemy_x_pos = 0;
            static int enemy_y_pos = 0;
            ImGui::InputInt("enemy x position", &enemy_x_pos);
            ImGui::InputInt("enemy y position", &enemy_y_pos);

            if (ImGui::Button("Create new enemy")) {
                Entity enemy = registry->create_entity();
                enemy.group("enemies");
                enemy.add_component<TransformComponent>(
                    glm::vec2(enemy_x_pos, enemy_y_pos), glm::vec2(1.0, 1.0),
                    0.0);
                enemy.add_component<RigidBodyComponent>(glm::vec2(0.0, 0.0));
                enemy.add_component<SpriteComponent>("tank-image", 32, 32, 2,
                                                     false);
                enemy.add_component<BoxColliderComponent>(32, 32);
                enemy.add_component<ProjectileEmitterComponent>(
                    glm::vec2(100.0, 0.0), 5000, 3000, 10, false);
                enemy.add_component<HealthComponent>(100);
            }
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    }
};

#endif
