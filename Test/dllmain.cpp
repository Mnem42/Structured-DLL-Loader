// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}
#define EXPORTING_DLL


extern "C" __declspec(dllexport) int a(void){
    return 42;
}
extern "C" __declspec(dllexport) LPCSTR b(int in) {
    if (in == 0) {
        return "A";
    }
    else {
        return "B";
    }
}
extern "C" __declspec(dllexport) void GetMetadataSize() {

}

#ifndef INDLL_H
#define INDLL_H
#ifdef EXPORTING_DLL
    //extern "C" __declspec(dllexport) int a();
#else
    extern "C" __declspec(dllimport) int a();
#endif

#endif

