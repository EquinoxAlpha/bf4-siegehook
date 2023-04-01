#pragma once

#include "sdk.h"
#include "draw_manager.h"
#include "xorstr.h"
#include "utils.h"

namespace features::misc {
	inline bool no_recoil{};
	inline bool no_spread{};
	inline bool no_sway{};

	inline void set_no_recoil() {
		auto soldier = utils::get_soldier();

		auto weapon = soldier->m_pWeaponComponent->GetActiveSoldierWeapon();
		if (!weapon) return;

		auto primary = weapon->m_pPrimary;
		if (!primary) return;

		auto sway = primary->m_Sway;
		if (!sway) return;

		auto data = sway->m_Data;
		if (!data) return;

		data->m_RecoilHip = 0.0f;
		data->m_RecoilSight = 0.0f;
	}

	inline void set_no_spread() {
		auto soldier = utils::get_soldier();

		auto weapon = soldier->m_pWeaponComponent->GetActiveSoldierWeapon();
		if (!weapon) return;

		auto primary = weapon->m_pPrimary;
		if (!primary) return;

		auto sway = primary->m_Sway;
		if (!sway) return;

		auto data = sway->m_Data;
		if (!data) return;

		data->m_SpreadHip = 0.0f;
		data->m_SpreadSight = 0.0f;
	}

	inline void preframe() {
		auto soldier = utils::get_soldier();

		if (no_recoil) {
			if (soldier) {
				set_no_recoil();
			}
		}
		if (no_spread) {
			if (soldier) {
				set_no_spread();
			}
		}
	}

	inline void unlock_all(bool state) {
		const auto settings = SyncedBFSettings::GetInstance();

		settings->m_AllUnlocksUnlocked = state;
	}

	inline void set_loadout() {
		const auto game_context = ClientGameContext::GetInstance();
		if (!game_context) return;

		const auto level = game_context->m_pLevel;
		ImGui::Text("Level ptr: 0x%X", level);
		if (!level) return;

		const auto team_data = level->m_ppTeamEntityData[1];
		ImGui::Text("Team data ptr: 0x%X", team_data);
		if (!team_data) return;

		const auto loadout = team_data->m_ppSoldierCustomization[1];
		ImGui::Text("Loadout ptr: 0x%X", loadout);
		if (!loadout) return;

		const auto weapon_table = loadout->m_pWeaponTable;
		ImGui::Text("Weapon table ptr: 0x%X", weapon_table);
		if (!weapon_table) return;

		auto unlocks = weapon_table->m_pUnlockParts;
		ImGui::Text("Unlocks ptr: 0x%X", unlocks);

		ImGui::Text("Unlocks table size: %i", unlocks.size());
	}

	inline void draw_menu() {
		if (ImGui::BeginTabItem(xorstr_("Miscellaneous"))) {
			ImGui::Checkbox(xorstr_("No recoil"), &no_recoil);
			ImGui::Checkbox(xorstr_("No spread"), &no_spread);
			//ImGui::Checkbox(xorstr_("No sway"), &no_sway);
			
			if (SyncedBFSettings::GetInstance()->m_AllUnlocksUnlocked) {
				if (ImGui::Button(xorstr_("Relock all"))) {
					unlock_all(false);
				}
			}
			else {
				if (ImGui::Button(xorstr_("Unlock all"))) {
					unlock_all(true);
				}
			}

			set_loadout();
				
			ImGui::EndTabBar();
		}
	}
}