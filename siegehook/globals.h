#pragma once

#include "math.h"
#include <windows.h>

class globals_class
{
public:
	vec2 g_window_size{};

	HANDLE g_main_thread{};
	HWND g_window{};
	HMODULE g_module{};

	bool g_fairfight{};
	bool g_punkbuster{};
	bool g_punkbuster_2{};

	mat4 g_viewmatrix{};

	vec3 g_view_position{};
};

bool world_to_screen(vec3 position, vec2& screen);

extern globals_class globals;