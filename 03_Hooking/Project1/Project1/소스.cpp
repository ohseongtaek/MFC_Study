#include "���.h"
#include <iostream>
#include <string.h>
#include <tchar.h>
#include <vector>

typedef int (WINAPI *MESSAGEBOXW)(HWND, LPCWSTR, LPCWSTR, UINT);
typedef int (WINAPI *SETLASTERROR)(DWORD);

// �Լ� ������ 
MESSAGEBOXW		   fpMessageBoxW		  = NULL;
SETLASTERROR	   fpSetLastError		  = NULL;

// Hooking �� �޽��� �ڽ� 
// ���� ���� �� ����
int WINAPI DetourMessageBoxW(HWND hWnd, LPCWSTR lpText, LPCWSTR lpCaption, UINT uType)
{
	return fpMessageBoxW(hWnd, L"Hooked!", lpCaption, uType);
}

int WINAPI DetourSetLastError(DWORD dwErrCode)
{
	return fpSetLastError(dwErrCode);
}

// ���� �Լ��� ���޵��� �ʰ� �����Ѱ����� �ٷ� ���� 
int WINAPI DetourGetLastError()
{
	return 100;
}

std::vector<HOOKSETDATA> vecHook = 
{
	{"User32.DLL","MessageBoxW",DetourMessageBoxW,&fpMessageBoxW},
	{"kernel32.DLL","SetLastError",DetourSetLastError,&fpSetLastError},
	{"kernel32.DLL","GetLastError",DetourGetLastError,NULL}
};


int main()
{
	DWORD nLoopCnt		   = 0;
	BOOL  SetMinHookResult = FALSE;
	INT   nError		   = 0;

	// Initialize MinHook.
	if (MH_Initialize() != MH_OK)
	{
		return 1;
	}
	
	// MinHook Create
	for (nLoopCnt = 0; nLoopCnt < vecHook.size(); nLoopCnt++)
	{
		SetMinHookResult = SetMinHook(&vecHook[nLoopCnt]);
		if (FALSE == SetMinHookResult)
		{
			std::cout << "Error " << std::endl;
		}
		SetMinHookResult = FALSE;
	}

	// MinHook Enable
	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return 1;
	}

	////////////////////////////////////////////// Test Code //////////////////////////////////////////////
	
	MessageBoxW(NULL, L"Not hooked...", L"MinHook Sample", MB_OK);

	nError = GetLastError();
	std::cout << "[Hook]First GetLastError = " << nError << std::endl;

	SetLastError(10);
	nError = GetLastError();
	std::cout << "[Hook]Second GetLastError = " << nError << std::endl;

	//////////////////////////////////////////////////////////////////////////////////////////////////////

	// MinHopok Disable
	if (MH_DisableHook(MH_ALL_HOOKS) != MH_OK)
	{
		return 1;
	}

	// MinHook Remove 
	for (nLoopCnt = 0; nLoopCnt < vecHook.size(); nLoopCnt++)
	{
		SetMinHookResult = UnSetMinHook(&vecHook[nLoopCnt]);
		if (FALSE == SetMinHookResult)
		{
			std::cout << "Error" << std::endl;
		}
		SetMinHookResult = FALSE;
	}

	////////////////////////////////////////////// Test Code //////////////////////////////////////////////
	MessageBoxW(NULL, L"Not hooked...", L"MinHook Sample", MB_OK);

	nError = GetLastError();
	std::cout << "[Not Hook]First GetLastError = " << nError << std::endl;

	SetLastError(1000);
	nError = GetLastError();
	std::cout << "[Not Hook]Second GetLastError = " << nError << std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// temp ���� ���� ���� ���� Sleep 
	Sleep(3000);

	// MinHook UnInit 
	if (MH_Uninitialize() != MH_OK)
	{
		return ERROR;
	}

	return 0;
}

DWORD SetMinHook(HOOKSETDATA *pvecHook)
{
	HMODULE		hModule = NULL;
	LPVOID		funcAddr = NULL;

	hModule = LoadLibrary(pvecHook->DllName);

	if (hModule == NULL)
	{
		return ERROR;
	}

	funcAddr = GetProcAddress(hModule, pvecHook->ModuleName);

	if (funcAddr == NULL)
	{
		return ERROR;
	}

	if (MH_OK != MH_CreateHook(funcAddr, pvecHook->MyPrcAddr, reinterpret_cast<void**>(pvecHook->MyOrgPrcAddr)))
	{
		return ERROR;
	}
	
	FreeLibrary(hModule);
	hModule = NULL;
	funcAddr = NULL;

	return TRUE;
}

DWORD UnSetMinHook(HOOKSETDATA *pvecHook)
{
	HMODULE		hModule = NULL;
	LPVOID		funcAddr = NULL;

	hModule = LoadLibrary(pvecHook->DllName);

	if (hModule == NULL)
	{
		return ERROR;
	}

	funcAddr = GetProcAddress(hModule, pvecHook->ModuleName);

	if (funcAddr == NULL)
	{
		return ERROR;
	}

	if (MH_OK != MH_RemoveHook(funcAddr))
	{
		return ERROR;
	}

	FreeLibrary(hModule);
	hModule = NULL;
	funcAddr = NULL;

	return TRUE;
}