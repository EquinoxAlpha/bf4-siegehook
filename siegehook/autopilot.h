#pragma once

#include "sdk.h"
#include "utils.h"

namespace features::autopilot {
	inline vec3 target_position{ -31.628553f, 269.633118f, 204.641876f };
	inline ClientVehicleEntity* target = nullptr;

	inline bool enabled = false;
	inline bool invert_controls = false;

	inline float fPitch = 0.0f;
	inline float fYaw = 0.0f;

	inline bool locked;

	inline void initialize() {
	}

	inline void select_target() {
		if (locked) return;

		const auto game_context = ClientGameContext::GetInstance();
		if (!game_context) return;

		const auto level = game_context->m_pLevel;
		if (!level) return;

		const auto game_world = level->m_pGameWorld;
		if (!game_world) return;

		if (class_info.VehicleEntity) {
			EntityIterator<ClientVehicleEntity> iterator(game_world, class_info.VehicleEntity);

			float closest_distance = FLT_MAX;
			target = nullptr;

			if (!iterator.front()) return;

			do {
				ClientVehicleEntity* vehicle = iterator.front()->getObject();
				if (!IsValidPtr(vehicle)) continue;


				vec3 position = vehicle->GetPosition();

				vec2 screen_center = { 960, 540 };
				vec2 w2s;

				if (!world_to_screen(position, w2s)) continue;

				float distance = (screen_center - w2s).magnitude();

				if (distance < closest_distance) {
					closest_distance = distance;
					target = vehicle;
				}
			} while (iterator.next());
		}
		else {
			update_class_info();
		}
	}

	inline float calcDelta(const vec3& vec, const vec3& delta, const float& FOV) {
		return std::asin(vec3::dot(vec, delta)) / FOV;
	}

	inline void predict_target_position(ClientVehicleEntity* jet) {
		if (!IsValidPtr(target)) return;


		vec3 position = target->GetCenter();
		vec3 local_position = jet->GetCenter();

		target_position = position;

		vec3 target_velocity = *target->GetVelocity();

		auto distance = (target_position - local_position).magnitude();

		auto weapon = VehicleTurret::GetInstance();
		if (!weapon) return;

		auto weapon_data = weapon->m_pWeaponFiring;
		if (!weapon_data) return;

		auto primary_fire = weapon_data->m_pPrimaryFire;
		if (!primary_fire) return;

		auto weapon_config = primary_fire->m_FiringData;
		if (!weapon_config) return;

		auto shot_data = weapon_config->m_ShotConfigData;

		auto speed = shot_data.m_Speed.magnitude();

		auto time = distance / speed;

		target_position = position + (target_velocity * time);
	}

	inline float re_range(float x, float inmin, float inmax, float outmin, float outmax)
	{
		return (x - inmin) * (outmax - outmin) / (inmax - inmin) + outmin;
	}

	inline void tick(ClientVehicleEntity* jet, float deltaTime) {
		if (!locked) { select_target(); }
		if (!IsValidPtr(target)) return;

		TransformAABBStruct transform; jet->GetAABB(&transform);

		auto fov = GameRenderer::GetInstance()->m_pRenderView->m_FovX;

		auto mat = VehicleTurret::GetInstance()->m_VehicleMatrix;
		vec3 left(mat(0,0), mat(0,1), mat(0,2));
		vec3 up(mat(1,0), mat(1,1), mat(1,2));

		//vec3 left(mat(0, 0), mat(1, 0), mat(2, 0));
		//vec3 up(mat(0, 1), mat(1, 1), mat(2, 1));

		//predict_target_position(jet);
		
		target_position = target->GetPosition();

		auto delta_position1 = (target_position - globals.g_view_position);
		auto delta_position = delta_position1.normalize();

		float yaw = calcDelta(left, delta_position, fov / 2.0f);
		float pitch = calcDelta(up, delta_position, fov / 2.0f);
		float roll = yaw;

		float delta;
		
		utils::get_screen_delta(target_position, delta);

		float sensitivity = re_range(delta, .5f, 200.f, 0.5f, 12.f);

		vec3 distance = target_position - globals.g_view_position;

		float invert = invert_controls ? -1.0f : 1.0f;

		utils::set_input(ConceptYaw, invert * -yaw * sensitivity);
		utils::set_input(ConceptPitch, invert * pitch * sensitivity);
		//utils::set_input(ConceptRoll, invert * roll * sensitivity * .85f);
	}

	inline void preframe(float deltaTime) {
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

		const auto local_vehicle = local_player->GetControlledVehicle();
		if (!local_vehicle) return;

		const auto vehicle_data = local_vehicle->m_Data;
		if (!vehicle_data) return;

		if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
			if (!locked) {
				select_target();
				locked = true;
			}
			else {
				locked = false;
			}
		}

		if (vehicle_data->IsTVMissile()) {
			__try {
				tick(local_vehicle, deltaTime);
			}
			__except (EXCEPTION_EXECUTE_HANDLER) {
				// do nothing
			}
		}
	}

	inline void draw_menu() {

		if (ImGui::BeginTabItem(xorstr_("Autopilot"))) {

			ImGui::Checkbox(xorstr_("Enabled"), &enabled);
			ImGui::Checkbox(xorstr_("Invert controls"), &invert_controls);

			ImGui::EndTabItem();
		}
	}

	inline void on_render() {

	}
}