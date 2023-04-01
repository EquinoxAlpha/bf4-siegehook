#pragma once

#include <Windows.h>
#include "sdk.h"

namespace utils {
	inline bool punkbuster_check() {
		auto ssmodule = (uintptr_t*)(OFFSET_SSMODULE);
		if (!IsValidPtr(ssmodule)) {
			return false;
		}

		bool status = (*(int*)(*ssmodule + 0x14) != -1);

		return status;
	}

	inline vec3 matrix_to_vec3(mat4 mat, int index) {
		return vec3(mat(0, index), mat(0, index + 1), mat(0, index + 2));
	}

	inline vec3 matrix_to_vec3_rows(mat4 mat, int index) {
		return vec3(mat(index, 0), mat(index, 1), mat(index, 2));
	}

	inline vec4 matrix_to_vec4(mat4 mat, int index) {
		return vec4(mat.m[index], mat.m[index + 1], mat.m[index + 2], mat.m[index + 3]);
	}

	inline bool get_screen_delta(vec3 pos, float& delta) {
		vec2 screen_center = { 960, 540 };
		vec2 w2s;

		if (world_to_screen(pos, w2s)) {
			vec2 delta2= w2s - screen_center;
			delta = delta2.magnitude();
			return true;
		}
		return false;
	}

	inline bool get_box_coords(TransformAABBStruct& aabb, vec4& coords) {
		vec3 corners[8];

		vec3 pos = aabb.Transform.position();
		vec3 min = aabb.AABB.m_Min.xyz();
		vec3 max = aabb.AABB.m_Max.xyz();

		
		corners[2] = pos + vec4(max.x, min.y, min.z, 0.0).transform(aabb.Transform).xyz();
		corners[3] = pos + vec4(max.x, min.y, max.z, 0.0).transform(aabb.Transform).xyz();
		corners[4] = pos + vec4(min.x, min.y, max.z, 0.0).transform(aabb.Transform).xyz();
		corners[5] = pos + vec4(min.x, max.y, max.z, 0.0).transform(aabb.Transform).xyz();
		corners[6] = pos + vec4(min.x, max.y, min.z, 0.0).transform(aabb.Transform).xyz();
		corners[7] = pos + vec4(max.x, max.y, min.z, 0.0).transform(aabb.Transform).xyz();
		min = pos + vec4(min.x, min.y, min.z, 0.0).transform(aabb.Transform).xyz();
		max = pos + vec4(max.x, max.y, max.z, 0.0).transform(aabb.Transform).xyz();
		corners[0] = min;
		corners[1] = max;

		for (auto& corner : corners) {
			vec2 s;
			if (!world_to_screen(corner, s)) {
				return false;
			}
		}

		coords.x = globals.g_window_size.x;
		coords.y = globals.g_window_size.y;
		coords.z = 0.0f;
		coords.w = 0.0f;

		for (auto& corner : corners) {
			vec2 s;
			if (!world_to_screen(corner, s)) {
				return false;
			}
			if (s.x < coords.x) {
				coords.x = s.x;
			}
			if (s.y < coords.y) {
				coords.y = s.y;
			}
			if (s.x > coords.z) {
				coords.z = s.x;
			}
			if (s.y > coords.w) {
				coords.w = s.y;
			}
		}

		return true;
	}

	inline void set_input(InputConceptIdentifiers code, float value) {
		auto border_input_node = BorderInputNode::GetInstance();
		if (!border_input_node) return;

		const auto input_cache = border_input_node->m_InputCache;
		if (!input_cache) return;

		const auto input = input_cache->m_Event;

		input[code] = value;

	}

	inline ClientSoldierEntity* get_soldier() {
		const auto game_context = ClientGameContext::GetInstance();
		if (!game_context) return nullptr;

		const auto level = game_context->m_pLevel;
		if (!level) return nullptr;

		const auto game_world = level->m_pGameWorld;
		if (!game_world) return nullptr;

		const auto player_manager = game_context->m_pPlayerManager;
		if (!player_manager) return nullptr;

		const auto local_player = player_manager->m_pLocalPlayer;
		if (!local_player) return nullptr;

		const auto local_soldier = local_player->GetSoldier();
		if (!local_soldier) return nullptr;

		if (!local_soldier->IsAlive()) return nullptr;

		return local_soldier;
	}

	inline bool raycast(vec3 from, vec3 to, RayCastHit& hit, int flags) {
		const auto client_game_context = ClientGameContext::GetInstance();
		if (!client_game_context) return false;

		const auto level = client_game_context->m_pLevel;
		if (!level) return false;

		const auto physics_manager = level->m_pHavokPhysics;
		if (!physics_manager) return false;
		
		const auto raycaster = physics_manager->m_pRayCaster;
		if (!raycaster) return false;

		eastl::fixed_vector<const void*, 8, 0> l;

		return raycaster->PhysicsRayQuery(xorstr_("6+f5989"), &from, &to, &hit, flags, &l);
	}
}