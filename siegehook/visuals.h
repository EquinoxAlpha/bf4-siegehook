#pragma once

#include <imgui.h>
#include "class_info.h"
#include "utils.h"
#include "draw_manager.h"
#include <vector>
#include "autopilot.h"

namespace features::visuals {
	inline bool enabled;

	inline vec3 color_friendly = vec3(0.0f, 1.0f, 0.0f);
	inline vec3 color_enemy = vec3(1.0f, 0.0f, 0.0f);
	inline vec3 color_hazard = vec3(1.0f, 1.0f, 0.0f);
	inline vec3 color_vehicle = vec3(1.0f, 1.0f, 1.0f);

	inline bool show_vehicles;
	inline bool show_players;
	inline bool show_explosives;
	inline bool show_unoccupied_vehicles;

	// Don't draw explosives that are further than this distance
	inline float explosive_distance = 100.0f;

	inline std::vector<ClientVehicleEntity*> occupied_vehicles;

	inline void draw_vehicle(ClientVehicleEntity* vehicle, vec3 color) {
		vec4 box;
		TransformAABBStruct aabb;

		vehicle->GetAABB(&aabb);

		if (utils::get_box_coords(aabb, box)) {
			if (vehicle == autopilot::target) {
				g_drawing.draw_text(box.xy(), "TARGET", 22.0f, color_hazard);
			}
			g_drawing.draw_box_with_corners(box.xy(), box.zw(), 0.55f, 0.55f, color);
		}
	}

	inline void draw_player(ClientSoldierEntity* soldier, vec3 color) {
		vec4 box;
		TransformAABBStruct aabb;

		soldier->GetAABB(&aabb);

		if (utils::get_box_coords(aabb, box)) {
			float health = soldier->m_pHealthComp->m_Health / soldier->m_pHealthComp->m_MaxHealth;
			float offset = 3;
			float width = 1;

			float delta = box.w - box.y;

			// Draw a health bar
			g_drawing.draw_box_filled(box.xy() - vec2(width + offset, 0.0f), box.xy() - vec2(offset, -delta), vec3(1.0f, 0.0f, 0.0f));
			g_drawing.draw_box_filled(box.xy() - vec2(offset, -delta * health), box.xy() - vec2(width + offset, 0.0f), vec3(0.0f, 1.0f, 0.0f));


			g_drawing.draw_box_with_corners(box.xy(), box.zw(), 0.6f, 0.5f, color);
		}
	}

	inline void draw_players() {
		occupied_vehicles.clear();
		const auto game_context = ClientGameContext::GetInstance();
		
		const auto local_player = game_context->m_pPlayerManager->m_pLocalPlayer;

		for (int i = 0; i < MAX_PLAYERS; i++) {
			const auto player = game_context->m_pPlayerManager->m_ppPlayers[i];
			if (!player) continue;
			if (player == local_player) continue;

			bool is_teammate = local_player->m_TeamId == player->m_TeamId;

			const auto soldier = player->GetSoldier();
			if (!soldier) continue;
			if (!soldier->IsAlive()) continue;

			const auto vehicle = player->GetVehicle();

			if (IsValidPtr(vehicle) && show_vehicles) {
				occupied_vehicles.push_back(vehicle);
				draw_vehicle(vehicle, is_teammate ? color_friendly : color_enemy);
			}
			else if (show_players) {
				draw_player(soldier, is_teammate ? color_friendly : color_enemy);
			}
		}
	}

	inline void draw_explosives(ClientGameWorld* world, ClientSoldierEntity* local_soldier) {
		if (class_info.ExplosionEntity) {
			EntityIterator<ClientExplosionEntity> explosives(world, class_info.ExplosionEntity);
			if (!explosives.front()) return;

			do {
				const auto explosive = explosives.front()->getObject();
				if (!IsValidPtr(explosive)) continue;

				ClientControllableEntity* explosive_controllable = (ClientControllableEntity*)explosive;

				TransformAABBStruct aabb;
				explosive->GetAABB(&aabb);

				TransformAABBStruct soldier_aabb;
				local_soldier->GetAABB(&soldier_aabb);

				vec3 pos = aabb.Transform.position();
				vec3 local_pos = soldier_aabb.Transform.position();

				if (pos.distance(local_pos) > explosive_distance) continue;

				vec4 box;
				if (utils::get_box_coords(aabb, box)) {
					g_drawing.draw_box_with_corners(box.xy(), box.zw(), 0.5f, 0.5f, color_hazard);

					vec2 center = (box.xy() + box.zw()) / 2.0f;

					g_drawing.set_current_font(1);
					g_drawing.draw_text_centered(center, "EXPLOSIVE", 16, color_hazard);
					g_drawing.pop_font();
				}

			} while (explosives.next());
		}
		else {
			update_class_info();
		}
	}

	inline void draw_vehicles(ClientGameWorld* world) {
		if (class_info.VehicleEntity) {
			EntityIterator<ClientVehicleEntity> vehicles(world, class_info.VehicleEntity);
			if (!vehicles.front()) return;

			do {
				const auto vehicle = vehicles.front()->getObject();
				if (!IsValidPtr(vehicle)) continue;

				if (std::find(occupied_vehicles.begin(), occupied_vehicles.end(), vehicle) != occupied_vehicles.end()) continue;

				draw_vehicle(vehicle, color_vehicle);
			} while (vehicles.next());
		}
		else {
			update_class_info();
		}
	}

	inline void on_render() {
		const auto game_context = ClientGameContext::GetInstance();
		if (!game_context) return;

		const auto level = game_context->m_pLevel;
		if (!level) return;

		const auto game_world = level->m_pGameWorld;
		if (!game_world) return;

		const auto player_manager = game_context->m_pPlayerManager;
		if (!player_manager) return;

		const auto local_player = player_manager->m_pLocalPlayer;
		if (!local_player) return;

		const auto local_soldier = local_player->GetSoldier();
		if (!local_soldier) return;

		if (!local_soldier->IsAlive()) return;

		if (enabled) {
			if (show_unoccupied_vehicles) {
				draw_vehicles(game_world);
			}
			if (show_players) {
				draw_players();
			}
			if (show_explosives) {
				draw_explosives(game_world, local_soldier);
			}
		}
	}

	inline void draw_menu() {
		if (ImGui::BeginTabItem(xorstr_("Visuals"))) {

			ImGui::Checkbox(xorstr_("Enabled"), &enabled);

			ImGui::ColorEdit3(xorstr_("Friendly Color"), &color_friendly.x, ImGuiColorEditFlags_PickerHueWheel);
			ImGui::ColorEdit3(xorstr_("Enemy Color"), &color_enemy.x, ImGuiColorEditFlags_PickerHueWheel);

			ImGui::Separator();

			ImGui::Checkbox(xorstr_("Show Players"), &show_players);
			
			ImGui::Separator();

			ImGui::Checkbox(xorstr_("Show Vehicles"), &show_vehicles);
			ImGui::Checkbox(xorstr_("Show Unoccupied Vehicles"), &show_unoccupied_vehicles);

			ImGui::Separator();


			ImGui::Checkbox(xorstr_("Show Explosives"), &show_explosives);
			ImGui::SliderFloat(xorstr_("Explosive Distance"), &explosive_distance, 0.f, 1000.f);
			ImGui::ColorEdit3(xorstr_("Explosives Color"), &color_hazard.x, ImGuiColorEditFlags_PickerHueWheel);

			ImGui::Separator();

			ImGui::EndTabItem();
		}
	}
}