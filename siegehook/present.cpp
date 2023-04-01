#include "hooking.h"
#include "renderer.h"
#include "globals.h"
#include "utils.h"

namespace hooking {
	bool is_clean = false;

	HRESULT __fastcall hkPresent(IDXGISwapChain* pChain, UINT syncInterval, UINT flags) {
		const auto renderer = DxRenderer::GetInstance();
		const auto game_renderer = GameRenderer::GetInstance();

		if (renderer && game_renderer)
		{
			globals.g_window_size = {(float)renderer->m_pScreen->m_Width, (float)renderer->m_pScreen->m_Height};
			globals.g_viewmatrix = game_renderer->m_pRenderView->m_ViewProjection;

			if (!renderer::initialized) {
				ID3D11Device* device = nullptr;
				ID3D11DeviceContext* context = nullptr;
				pChain->GetDevice(__uuidof(ID3D11Device), (void**)&device);
				device->GetImmediateContext(&context);
				renderer::device = device;
				renderer::context = context;
				renderer::initialize(pChain);
			}

			if (utils::punkbuster_check()) {
				return inline_present.call<HRESULT>(pChain, syncInterval, flags);
			}

			bool draw = !globals.g_punkbuster && !globals.g_fairfight && !globals.g_punkbuster_2;

			if (draw) {
				renderer::on_present();
				renderer::is_clean = false;
			}
			else {
				renderer::frames_skipped++;
				renderer::is_clean = true;
			}
		}

		return inline_present.call<HRESULT>(pChain, syncInterval, flags);
	}
}