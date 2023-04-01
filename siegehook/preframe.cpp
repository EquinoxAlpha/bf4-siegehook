#include "hooking.h"
#include "features.h"

namespace hooking {
	void hkPreframe(float deltaTime) {
		oPreFrameUpdate(deltaTime);

		auto game_renderer = GameRenderer::GetInstance();
		auto view_inverse = game_renderer->m_pRenderView->m_ViewInverse;
		globals.g_view_position = vec3(view_inverse(3, 0), view_inverse(3, 1), view_inverse(3, 2));

		features::on_pre_frame(deltaTime);
	}
}