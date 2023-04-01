#include "common.h"
#include "hooking.h"
#include "globals.h"
#include <iostream>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        globals.g_module = hModule;
		DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, NULL, [](PVOID) -> DWORD {

            globals.g_window = FindWindowA(xorstr_("Battlefield 4"), NULL);

            //AllocConsole();
            //FILE* fp;
            //freopen_s(&fp, "CONOUT$", "w", stdout);

            hooking::initialize();

            while (!GetAsyncKeyState(VK_END))
				Sleep(100);

            //logger.log("Exiting");

            hooking::terminate();

            //FreeConsole();
            FreeLibraryAndExitThread(globals.g_module, 0);

            return 0;
        }, NULL, NULL, NULL);
	}

    return TRUE;
}