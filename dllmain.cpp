// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "DLLBody.hpp"

BOOL APIENTRY DllMain
(
    HMODULE hModule,
    DWORD  reason,
    LPVOID /*lpReserved*/
)
{
    switch (reason)
    {
        case DLL_PROCESS_ATTACH:
            return StartInstance() ? TRUE : FALSE;
            break;

        case DLL_PROCESS_DETACH:
            EndInstance();
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            break;
    }
    return TRUE;
}
