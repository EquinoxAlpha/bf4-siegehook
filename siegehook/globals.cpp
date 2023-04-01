#include "globals.h"

globals_class globals;

bool world_to_screen(vec3 position, vec2& screen) {
	return math::world_to_screen(position, globals.g_window_size, globals.g_viewmatrix, screen);
}