#pragma once

#include <imgui.h>
#include "math.h"

class DrawManager
{
public:
	inline static float line_width = 2.0f;

	inline ImDrawList* draw_list() {
		return ImGui::GetBackgroundDrawList();
	}

	void set_current_font(int index);

	void pop_font();

	void draw_box(vec2 min, vec2 max, vec3 color = vec3(1.0, 1.0, 1.0));

	void draw_box_filled(vec2 min, vec2 max, vec3 color = vec3(1.0, 1.0, 1.0));

	void draw_box_filled_shadow(vec2 min, vec2 max, vec3 color = vec3(1.0, 1.0, 1.0));

	void draw_line(vec2 start, vec2 end, vec3 color = vec3(1.0, 1.0, 1.0));

	void draw_text(vec2 pos, const char* text, float size = 12, vec3 color = vec3(1.0, 1.0, 1.0));

	void draw_text_centered(vec2 pos, const char* text, float size = 12, vec3 color = vec3(1.0, 1.0, 1.0));

	void draw_circle(vec2 pos, float radius, vec3 color = vec3(1.0, 1.0, 1.0));

	void draw_box_with_corners(vec2 min, vec2 max, float x_space = 0.6f, float y_space = 0.5f, vec3 color = vec3(1.0, 1.0, 1.0));
};

extern DrawManager g_drawing;