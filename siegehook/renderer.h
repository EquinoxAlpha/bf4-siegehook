#pragma once

#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include "globals.h"
#include "gui.h"
#include <imgui_impl_win32.h>
#include "features.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace renderer {

	inline IDXGISwapChain* swapchain = nullptr;
	inline ID3D11Device* device = nullptr;
	inline ID3D11DeviceContext* context = nullptr;

	inline bool initialized = false;
	inline bool is_clean = false;

	inline int frames_skipped = 0;

	void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void initialize(IDXGISwapChain* pChain);
	void shutdown();
	void mouse_act();
	void on_present();
	void pre_reset();
	void post_reset();
}