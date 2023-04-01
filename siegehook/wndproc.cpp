#include "hooking.h"
#include "renderer.h"

namespace hooking {
	LRESULT hkWndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (renderer::initialized) {
			renderer::wndproc(hwnd, msg, wparam, lparam);

			switch (msg) {
			case WM_SIZE:
				if (renderer::device != nullptr && wparam != SIZE_MINIMIZED) {
					renderer::pre_reset();
					renderer::swapchain->ResizeBuffers(0, (UINT)LOWORD(lparam), (UINT)HIWORD(lparam), DXGI_FORMAT_UNKNOWN, 0);
					renderer::post_reset();
				}
				return false;
			}

			if (gui::open) {
				switch (msg) {
				case WM_MOUSEMOVE:
					return false;
				default:
					break;
				}
				return true;
			}
		}
		return CallWindowProcW(hooking::o_wndproc, hwnd, msg, wparam, lparam);
	}
}