#include "renderer.h"
#include "ttf-ruda.h"

namespace renderer {
	void wndproc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
		if (msg == WM_KEYUP && wparam == VK_F1)
		{
			gui::open ^= true;
		}

		ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam);
	}

	void initialize(IDXGISwapChain* pChain) {
		swapchain = pChain;

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;

		ImGui::StyleColorsDark();

		ImGui_ImplDX11_Init(device, context);
		ImGui_ImplWin32_Init(globals.g_window);

		io.Fonts->AddFontFromFileTTF(xorstr_("C:\\Users\\equi\\Downloads\\RobotoMono-Regular.ttf"), 14.0f);
		io.Fonts->AddFontFromFileTTF(xorstr_("C:\\Users\\equi\\Downloads\\RobotoMono-Medium.ttf"), 14.0f);
		//io.Fonts->AddFontFromMemoryCompressedBase85TTF(ttf_ruda_regular_compressed_data_base85, 16.0f);

		logger.log(xorstr_("Initialized ImGui!\n"));

		initialized = true;
	}

	void shutdown() {
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		initialized = false;
	}

	void mouse_act()
	{
		const auto input = BorderInputNode::GetInstance();

		if (IsValidPtr(input) && IsValidPtr(input->m_pMouse) && IsValidPtr(input->m_pMouse->m_pDevice))
		{
			if (!input->m_pMouse->m_pDevice->m_CursorMode)
			{
				input->m_pMouse->m_pDevice->m_UIOwnsInput, input->m_pMouse->m_pDevice->m_UseRawMouseInput, ImGui::GetIO().MouseDrawCursor = gui::open;
			}
		}
	}

	void on_present() {
		if (!initialized)
			return;

		if (gui::open)
		{
			ImGui::GetIO().MouseDrawCursor = true;
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
		else
		{
			ImGui::GetIO().MouseDrawCursor = false;
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
		}

		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		mouse_act();
		features::on_render();
		if (gui::open) {
			{
				const auto input = BorderInputNode::GetInstance();

				if (input) {
					const auto mouse = input->m_pMouse;
					if (mouse) {
						const auto device = mouse->m_pDevice;
						if (device) {
							device->m_UIOwnsInput = true;
							device->m_UseRawMouseInput = true;
						}
					}
				}
			}

			ImGui::Begin(xorstr_("menu"));
			ImGui::Text("Frames skipped: %i", frames_skipped);
			gui::on_render();
			ImGui::End();
		}

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	void pre_reset() {
		ImGui_ImplDX11_InvalidateDeviceObjects();
	}

	void post_reset() {
		ImGui_ImplDX11_CreateDeviceObjects();
	}
}