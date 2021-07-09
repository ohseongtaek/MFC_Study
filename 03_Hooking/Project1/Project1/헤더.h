#pragma once

#include "MinHook.h"
#include <Windows.h>

#if defined _M_X64
#pragma comment(lib, "libMinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "libMinHook.x86.lib")
#endif

template <typename T>
inline MH_STATUS MH_CreateHookEx(LPVOID pTarget, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHook(pTarget, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}

template <typename T>
inline MH_STATUS MH_CreateHookApiEx(
	LPCWSTR pszModule, LPCSTR pszProcName, LPVOID pDetour, T** ppOriginal)
{
	return MH_CreateHookApi(
		pszModule, pszProcName, pDetour, reinterpret_cast<LPVOID*>(ppOriginal));
}


// seongtaek User Struct
typedef struct _HOOKSETDATA
{
	CHAR DllName[32];
	CHAR ModuleName[64];
	PVOID MyPrcAddr;
	PVOID MyOrgPrcAddr;
} HOOKSETDATA;

// seongtaek User Function 
DWORD SetMinHook(HOOKSETDATA *pvecHook);
DWORD UnSetMinHook(HOOKSETDATA *pvecHook);