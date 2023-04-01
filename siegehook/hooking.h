#pragma once
#include <SafetyHook.hpp>
#include <windows.h>
#include <d3d11.h>
#include "sdk.h"

typedef LRESULT(CALLBACK* WndProc_t)(HWND, UINT, WPARAM, LPARAM);

namespace hooking
{
	using PreFrameUpdate_t = void(*)(float dt);

	inline SafetyHookInline inline_bitblt;
	inline SafetyHookInline inline_present;
	inline SafetyHookInline inline_dispatchmessage;
	inline SafetyHookInline inline_copysubresourceregion;
	inline WndProc_t o_wndproc;
	inline PreFrameUpdate_t oPreFrameUpdate = nullptr;

	void initialize();
	void terminate();

	BOOL hkBitBlt(HDC hdcDst, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop);
	HRESULT hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
	void hkDispatchMessage(MessageManager* pMessageManagerImpl, Message* pMessage);
	void hkCopySubresourceRegion(ID3D11DeviceContext* pContext, ID3D11Resource* pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D11Resource* pSrcResource, UINT SrcSubresource, const D3D11_BOX* pSrcBox);
	LRESULT hkWndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	void hkPreframe(float dt);

	PBYTE hook_vtable_func(PDWORD64* vtable, PBYTE hook, SIZE_T index);
};

