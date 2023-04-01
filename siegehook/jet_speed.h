#pragma once

#include "common.h"

namespace features::jet_speed {
	inline bool enabled = false;
	inline float speed = 314.f;
	inline float margin = 6.0f;

	inline void correct_for_velocity(float target, ClientVehicleEntity* vehicle) {
		vec3 vehicle_velocity = *vehicle->GetVelocity();
		auto velocity = vehicle_velocity.magnitude() * 3.6f;

		float target_speed = speed;
		float lowest_target_speed = target_speed - margin;
		float highest_target_speed = target_speed + margin;

		if (velocity < target_speed && velocity > lowest_target_speed)
		{
			utils::set_input(ConceptMoveForward, 1.0f);
			utils::set_input(ConceptMoveFB, 1.0f);
			utils::set_input(ConceptFreeCameraMoveFB, 1.0f);
			utils::set_input(ConceptFreeCameraTurboSpeed, 0.0f);
			utils::set_input(ConceptSprint, 0.0f);
		}
		else if (velocity >= highest_target_speed)
		{
			utils::set_input(ConceptMoveBackward, 1.0f);
			utils::set_input(ConceptMoveFB, -1.0f);
			utils::set_input(ConceptFreeCameraMoveFB, -1.0f);
			utils::set_input(ConceptFreeCameraTurboSpeed, 0.0f);
			utils::set_input(ConceptSprint, 0.0f);
			utils::set_input(ConceptBrake, 1.0f);
			utils::set_input(ConceptCrawl, 1.0f);
		}
		else if (velocity <= lowest_target_speed)
		{
			utils::set_input(ConceptMoveForward, 1.0f);
			utils::set_input(ConceptMoveFB, 1.0f);
			utils::set_input(ConceptFreeCameraMoveFB, 1.0f);
			utils::set_input(ConceptFreeCameraTurboSpeed, 1.0f);
			utils::set_input(ConceptSprint, 1.0f);
		}
	}

	inline void preframe() {
		if (!enabled) return;

		auto border_input_node = BorderInputNode::GetInstance();
		if (!border_input_node) return;

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

		const auto local_vehicle = local_player->GetVehicle();
		if (!local_vehicle) return;

		const auto vehicle_data = local_vehicle->m_Data;
		if (!vehicle_data) return;

		if (vehicle_data->IsInJet()) {
			correct_for_velocity(speed, local_vehicle);
		}
	}

	inline void draw_menu() {
		if (ImGui::BeginTabItem(xorstr_("Jet Speed"))) {
			ImGui::Checkbox(xorstr_("Control jet speed"), &enabled);
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip(xorstr_("Tries to keep a jet's speed at the target speed."));
			}
			ImGui::SliderFloat(xorstr_("Target speed"), &speed, 200.f, 400.f, xorstr_("%.0f"));
			ImGui::SliderFloat(xorstr_("Margin"), &margin, 0.1f, 20.f, xorstr_("%.1f"));
			if (ImGui::IsItemHovered()) {
				ImGui::SetTooltip(xorstr_("How large of a difference between the target speed and current speed is allowed."));
			}
			ImGui::EndTabItem();
		}
	}
}