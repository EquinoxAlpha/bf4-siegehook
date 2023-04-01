#pragma once

#include <imgui.h>
#include "features.h"

namespace gui {
	inline bool open = false;

	inline void on_render() {

		ImGui::BeginTabBar(xorstr_("Tab bar"));

		features::visuals::draw_menu();
		features::jet_speed::draw_menu();
		features::autopilot::draw_menu();
		features::misc::draw_menu();

		ImGui::EndTabBar();
	}
}