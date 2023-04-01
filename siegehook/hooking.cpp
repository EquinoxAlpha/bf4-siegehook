#include "hooking.h"
#include "globals.h"
#include <d3d11.h>
#include "sdk.h"

PBYTE hooking::hook_vtable_func(PDWORD64* vtable, PBYTE hook, SIZE_T index) {
	DWORD dwOld = 0;
	VirtualProtect((void*)((*vtable) + index), sizeof(PDWORD64), PAGE_EXECUTE_READWRITE, &dwOld);

	PBYTE pOrig = ((PBYTE)(*vtable)[index]);
	(*vtable)[index] = (DWORD64)hook;
	VirtualProtect((void*)((*vtable) + index), sizeof(PDWORD64), dwOld, &dwOld);

	return pOrig;

}

void hooking::initialize()
{
	auto builder = SafetyHookFactory::acquire();
	auto renderer = DxRenderer::GetInstance();
	auto border_input_node = BorderInputNode::GetInstance();

	inline_bitblt = builder.create_inline(BitBlt, hkBitBlt);

	auto vtable = *reinterpret_cast<uintptr_t**>(renderer->m_pScreen->m_pSwapChain);
	auto present = reinterpret_cast<void*>(vtable[8]);

	inline_present = builder.create_inline(present, hkPresent);

	vtable = *reinterpret_cast<uintptr_t**>(renderer->m_pContext);
	auto copysubregion = reinterpret_cast<void*>(vtable[46]);

	oPreFrameUpdate = reinterpret_cast<PreFrameUpdate_t>(hook_vtable_func(reinterpret_cast<PDWORD64*>(border_input_node->m_Vtable), reinterpret_cast<PBYTE>(&hkPreframe), 3));

	//inline_dispatchmessage = builder.create_inline((void*)OFFSET_DISPATCHMESSAGE, hkDispatchMessage);
	inline_copysubresourceregion = builder.create_inline(copysubregion, hkCopySubresourceRegion);

	o_wndproc = (WNDPROC)SetWindowLongPtr(globals.g_window, GWLP_WNDPROC, (LONG_PTR)hkWndproc);
}

void hooking::terminate()
{
	inline_bitblt = {};
	inline_present = {};
	inline_dispatchmessage = {};
	inline_copysubresourceregion = {};

	hook_vtable_func(reinterpret_cast<PDWORD64*>(BorderInputNode::GetInstance()->m_Vtable), reinterpret_cast<PBYTE>(oPreFrameUpdate), 3);

	SetWindowLongPtr(globals.g_window, GWLP_WNDPROC, (LONG_PTR)o_wndproc);
}
