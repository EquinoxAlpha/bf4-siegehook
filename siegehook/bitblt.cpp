#include "hooking.h"
#include "globals.h"
#include "renderer.h"

namespace hooking {
	BOOL hkBitBlt(HDC hdcDst, int x, int y, int cx, int cy, HDC hdcSrc, int x1, int y1, DWORD rop) {
		logger.log("WARNING! Screenshot (FairFight) detected. Disabling GUI for 50ms.");
		globals.g_fairfight = true;

		Sleep(50);

		bool result = inline_bitblt.call<BOOL>(hdcDst, x, y, cx, cy, hdcSrc, x1, y1, rop);

		Sleep(20);

		globals.g_fairfight = false;

		return result;
	}
}