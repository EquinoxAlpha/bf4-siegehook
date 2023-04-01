#include "draw_manager.h"

void DrawManager::set_current_font(int index) {
	ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[index]);
}

void DrawManager::pop_font() {
	ImGui::PopFont();
}

void DrawManager::draw_box(vec2 min, vec2 max, vec3 color) {
	ImGui::GetBackgroundDrawList()->AddRect(ImVec2(min.x, min.y), ImVec2(max.x, max.y), ImColor(color.x, color.y, color.z), 0, 0, line_width);
}

void DrawManager::draw_box_filled(vec2 min, vec2 max, vec3 color) {
	ImGui::GetBackgroundDrawList()->AddRectFilled(ImVec2(min.x, min.y), ImVec2(max.x, max.y), ImColor(color.x, color.y, color.z));
}

void DrawManager::draw_box_filled_shadow(vec2 min, vec2 max, vec3 color)
{
	// Draw the shadow
	draw_box_filled(vec2(min.x + 1, min.y + 1), vec2(max.x + 1, max.y + 1), vec3(0, 0, 0));
	// Draw the box
	draw_box_filled(min, max, color);
}

void DrawManager::draw_line(vec2 start, vec2 end, vec3 color) {
	ImGui::GetBackgroundDrawList()->AddLine(ImVec2(start.x, start.y), ImVec2(end.x, end.y), ImColor(color.x, color.y, color.z), line_width);
}

// Draw text on the screen.
// pos: The position of the text.
// text: The text to draw.
// size: The size of the text. (optional, defaults to 12)
// color: The color of the text. (optional, defaults to white)
void DrawManager::draw_text(vec2 pos, const char* text, float size, vec3 color) {
	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), size, ImVec2(pos.x, pos.y), ImColor(color.x, color.y, color.z), text);
}

void DrawManager::draw_text_centered(vec2 pos, const char* text, float size, vec3 color) {
	ImFont* font = ImGui::GetFont();
	ImVec2 text_size = font->CalcTextSizeA(size, FLT_MAX, 0.0f, text);
	draw_text(pos - vec2(text_size.x / 2, text_size.y / 2), text, size, color);
}

void DrawManager::draw_circle(vec2 pos, float radius, vec3 color) {
	ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(pos.x, pos.y), radius, ImColor(color.x, color.y, color.z));
}

void DrawManager::draw_box_with_corners(vec2 min, vec2 max, float x_space, float y_space, vec3 color)
{
	// Determines the empty space between each corner or line.

	vec2 size = max - min;

	float x_line_width = size.x * (1 - x_space) / 2;
	float y_line_width = size.y * (1 - y_space) / 2;

	vec3 outline_color = vec3(0, 0, 0);

	line_width = 4.0f;

	// Top left corner
	draw_line(vec2(min.x, min.y), vec2(min.x + x_line_width, min.y), outline_color);
	draw_line(vec2(min.x, min.y), vec2(min.x, min.y + y_line_width), outline_color);

	// Top right corner
	draw_line(vec2(max.x, min.y), vec2(max.x - x_line_width, min.y), outline_color);
	draw_line(vec2(max.x, min.y), vec2(max.x, min.y + y_line_width), outline_color);

	// Bottom left corner
	draw_line(vec2(min.x, max.y), vec2(min.x + x_line_width, max.y), outline_color);
	draw_line(vec2(min.x, max.y), vec2(min.x, max.y - y_line_width), outline_color);

	// Bottom right corner
	draw_line(vec2(max.x, max.y), vec2(max.x - x_line_width, max.y), outline_color);
	draw_line(vec2(max.x, max.y), vec2(max.x, max.y - y_line_width), outline_color);

	line_width = 2.0f;

	// Top left corner
	draw_line(vec2(min.x, min.y), vec2(min.x + x_line_width, min.y), color);
	draw_line(vec2(min.x, min.y), vec2(min.x, min.y + y_line_width), color);

	// Top right corner
	draw_line(vec2(max.x, min.y), vec2(max.x - x_line_width, min.y), color);
	draw_line(vec2(max.x, min.y), vec2(max.x, min.y + y_line_width), color);

	// Bottom left corner
	draw_line(vec2(min.x, max.y), vec2(min.x + x_line_width, max.y), color);
	draw_line(vec2(min.x, max.y), vec2(min.x, max.y - y_line_width), color);

	// Bottom right corner
	draw_line(vec2(max.x, max.y), vec2(max.x - x_line_width, max.y), color);
	draw_line(vec2(max.x, max.y), vec2(max.x, max.y - y_line_width), color);
}

DrawManager g_drawing;