#ifndef INCLUDED_RENDER_GUI_SYSTEM_H
#define INCLUDED_RENDER_GUI_SYSTEM_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <libs/imgui/imgui.h>
#include <libs/imgui/imgui_impl_sdl2.h>
#include <libs/imgui/imgui_impl_sdlrenderer2.h>

#include <cmath>
#include <glm/ext/vector_float2.hpp>
#include <glm/trigonometric.hpp>
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
                const std::unique_ptr<Registry>& registry,
                const SDL_Rect& camera) {
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (ImGui::Begin("Spawn enemies")) {
            // ImGui::Text("Here is where we spawn new enemies...");
            static int pos_x = 0;
            static int pos_y = 0;
            static int scale_x = 0;
            static int scale_y = 0;
            static int vel_x = 0;
            static int vel_y = 0;
            static int health = 100;
            static float rotation = 0.0;
            static float proj_angle = 0.0;
            static float proj_speed = 100.0;
            static int proj_repeat = 10;
            static int proj_duration = 10;
            const char* sprites[] = {"tank-image", "truck-image"};
            static int selected_sprite_index = 0;

            // Section to input enemy sprite texture ID
            if (ImGui::CollapsingHeader("Sprite",
                                        ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Combo("texture ID", &selected_sprite_index, sprites,
                             IM_ARRAYSIZE(sprites));
            }
            ImGui::Spacing();

            // Section to input enemy transform values
            if (ImGui::CollapsingHeader("Transform",
                                        ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputInt("position x", &pos_x);
                ImGui::InputInt("position y", &pos_y);
                ImGui::SliderInt("scale x", &scale_x, 1, 10);
                ImGui::SliderInt("scale y", &scale_y, 1, 10);
                ImGui::SliderAngle("rotation (deg)", &rotation, 0, 360);
            }
            ImGui::Spacing();

            // Section to input enemy rigid body values
            if (ImGui::CollapsingHeader("Rigid body",
                                        ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::InputInt("velocity x", &vel_x);
                ImGui::InputInt("velocity y", &vel_y);
            }
            ImGui::Spacing();

            // Section to input enemy projectile emitter values
            if (ImGui::CollapsingHeader("Projectile emitter",
                                        ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderAngle("angle (deg)", &proj_angle, 0, 360);
                ImGui::SliderAngle("speed (px/sec)", &proj_speed, 10, 500);
                ImGui::InputInt("repeat (sec)", &proj_repeat);
                ImGui::InputInt("duration (sec)", &proj_duration);
            }
            ImGui::Spacing();

            // Section to input enemy health values
            if (ImGui::CollapsingHeader("Health",
                                        ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::SliderInt("%", &health, 0, 100);
            }
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::Spacing();

            if (ImGui::Button("Create new enemy")) {
                Entity enemy = registry->create_entity();
                enemy.group("enemies");
                enemy.add_component<TransformComponent>(
                    glm::vec2(pos_x, pos_y), glm::vec2(scale_x, scale_y),
                    glm::degrees(rotation));
                enemy.add_component<RigidBodyComponent>(
                    glm::vec2(vel_x, vel_y));
                enemy.add_component<SpriteComponent>(
                    sprites[selected_sprite_index], 32, 32, 2);
                enemy.add_component<BoxColliderComponent>(32, 32,
                                                          glm::vec2(5, 5));
                double proj_vel_x = cos(proj_angle) * proj_speed;
                double proj_vel_y = sin(proj_angle) * proj_speed;
                enemy.add_component<ProjectileEmitterComponent>(
                    glm::vec2(proj_vel_x, proj_vel_y), proj_repeat * 1000,
                    proj_duration * 1000, 10, false);
                enemy.add_component<HealthComponent>(health);

                // RESET all input values after we create a new enemy
                pos_x = pos_y = rotation = proj_angle = 0;
                scale_x = scale_y = 1;
                proj_repeat = proj_duration = 10;
                proj_speed = 100;
                health = 100;
            }
        }
        ImGui::End();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                                        ImGuiWindowFlags_AlwaysAutoResize |
                                        ImGuiWindowFlags_NoNav;
        ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Always, ImVec2(0, 0));
        ImGui::SetNextWindowBgAlpha(0.9f);
        if (ImGui::Begin("Map coordinates", NULL, window_flags)) {
            ImGui::Text("Map coordinates (x = %.1f, y=%.1f)",
                        ImGui::GetIO().MousePos.x + camera.x,
                        ImGui::GetIO().MousePos.y + camera.y);
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
    }
};

#endif
