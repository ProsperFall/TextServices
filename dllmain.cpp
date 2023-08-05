#include "windows.h"
#include "TextServices.h"

BOOL APIENTRY DllMain(HMODULE hInstance,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        TextServices::dllInstanceHandle = hInstance;

        if (!InitializeCriticalSectionAndSpinCount(&TextServices::TextServiceCriticalSection, 0))
        {
            return FALSE;
        }

        /* if (!Global::RegisterWindowClass()) {
             return FALSE;
         }*/
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
