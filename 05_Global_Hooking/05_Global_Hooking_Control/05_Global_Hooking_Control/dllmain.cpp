#include "dllheader.h"
#include "pch.h"
#include <iostream>
#include <TlHelp32.h>
#include "MinHook.h"
#include <map>
//#include "../../05_gzPrintInject/05_gzPrintInject/소스.cpp"

#pragma comment(lib, "libMinHook.x64.lib")

typedef INT(WINAPI *MessageBoxA_Proc)(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType);
typedef LPSTR(WINAPI *CharNextA_Proc)(LPCSTR lpsz);
typedef VOID(WINAPI *GetLocalTime_Proc)(LPSYSTEMTIME lpSystemTime);
typedef BOOL(WINAPI *GetSystemTimes_Proc)(PFILETIME lpIdleTime, PFILETIME lpKernelTime, PFILETIME lpUserTime);
typedef DWORD(WINAPI *GetFullPathNameA_Proc)(LPCSTR lpFileName, DWORD nBufferLength, LPSTR lpBuffer, LPSTR *lpFilePart);
typedef HANDLE(WINAPI *CreateToolhelp32Snapshot_Proc)(DWORD dwFlags, DWORD th32ProcessID);
typedef BOOL(WINAPI *Process32First_Proc)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
typedef BOOL(WINAPI *Process32Next_Proc)(HANDLE hSnapshot, LPPROCESSENTRY32 lppe);
typedef HANDLE(WINAPI *CreateFileA_Proc)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef BOOL(WINAPI *WritePrivateProfileString_Proc)(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString, LPCWSTR lpFileName);
typedef DWORD(WINAPI *GetPrivateProfileString_Proc)(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpDefault, LPTSTR  lpReturnedString, DWORD nSize, LPCTSTR lpFileName);
typedef UINT(WINAPI *GetPrivateProfileInt_Proc)(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName);

MessageBoxA_Proc				fpMessageBox				= NULL;
CharNextA_Proc					fpCharNextA					= NULL;
GetLocalTime_Proc				fpGetLocalTime				= NULL;
GetSystemTimes_Proc				fpGetSystemTimes			= NULL;
GetFullPathNameA_Proc			fpGetFullPathNameA			= NULL;
CreateToolhelp32Snapshot_Proc	fpCreateToolhelp32Snapshot	= NULL;
Process32First_Proc				fpProcess32First			= NULL;
Process32Next_Proc				fpProcess32Next				= NULL;
CreateFileA_Proc			    fpCreateFileA				= NULL;
WritePrivateProfileString_Proc	fpWritePrivateProfileString = NULL;
GetPrivateProfileString_Proc    fpGetPrivateProfileString	= NULL;
GetPrivateProfileInt_Proc		fpGetPrivateProfileInt		= NULL;

// 만약 다른 핸들에 같은 이벤트를 주고싶은 경우에 아래와 같이 Flag 설정 후 한번 더 동작하게 유도 
BOOL g_bTest				= TRUE;
BOOL g_PrivateProfile_Flag	= FALSE;

INT WINAPI _MessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
	DWORD				dwReturnValue = 0;
	MessageBoxA_Proc	MessageBoxA_Orig = NULL;

	lpText	  = "[1]Hooking Text";
	lpCaption = "[1]Hooking Caption";

	MessageBoxA_Orig = fpMessageBox;

	if (g_bTest)
	{
		MessageBoxA_Orig(hWnd, lpText, lpCaption, uType);
	}

	dwReturnValue = (DWORD)MessageBoxA_Orig(hWnd, lpText, lpCaption, uType);

	return (BOOL)dwReturnValue;
}

LPSTR WINAPI _CharNextA(LPCSTR lpsz)
{
	LPSTR szRtn = NULL;
	CharNextA_Proc CharNextA_Orig = NULL;
	CharNextA_Orig = fpCharNextA;

	lpsz = "Hooking";

	if (g_bTest)
	{
		szRtn = CharNextA_Orig(lpsz);
	}

	szRtn = CharNextA_Orig(lpsz);

	return szRtn;
}

VOID WINAPI _GetLocalTime(LPSYSTEMTIME lpSystemTime)
{
	GetLocalTime_Proc	GetLocalTime_Orig = NULL;
	GetLocalTime_Orig = fpGetLocalTime;

	if (g_bTest)
	{
		GetLocalTime_Orig(lpSystemTime);
	}

	GetLocalTime_Orig(lpSystemTime);

	lpSystemTime->wYear = 1996;
	lpSystemTime->wMonth = 6;
	lpSystemTime->wDay = 14;
	lpSystemTime->wHour = 2;
	lpSystemTime->wMinute = 30;
	
	return;
}

BOOL WINAPI _GetSystemTimes(PFILETIME lpIdleTime, PFILETIME lpKernelTime, PFILETIME lpUserTime)
{
	BOOL bRtn = FALSE;
	GetSystemTimes_Proc GetSystemTimes_Orig = NULL;
	GetSystemTimes_Orig = fpGetSystemTimes;

	if (g_bTest)
	{
		GetSystemTimes_Orig(lpIdleTime, lpKernelTime, lpUserTime);
	}

	bRtn = GetSystemTimes_Orig(lpIdleTime, lpKernelTime, lpUserTime);

	lpIdleTime->dwHighDateTime	 = 1;
	lpIdleTime->dwLowDateTime	 = 2;
	lpKernelTime->dwHighDateTime = 3;
	lpKernelTime->dwLowDateTime  = 4;

	return bRtn;
}

DWORD _GetFullPathNameA(LPCSTR lpFileName, DWORD  nBufferLength, LPSTR  lpBuffer, LPSTR  *lpFilePart)
{
	DWORD bRtn = FALSE;
	GetFullPathNameA_Proc GetFullPathNameA_Orig = NULL;
	GetFullPathNameA_Orig = fpGetFullPathNameA;

	lpFileName = "HookingSuccess";

	if (g_bTest)
	{
		bRtn = GetFullPathNameA_Orig(lpFileName, nBufferLength, lpBuffer, lpFilePart);
	}

	bRtn = GetFullPathNameA_Orig(lpFileName, nBufferLength, lpBuffer, lpFilePart);

	return (BOOL)bRtn;
}

HANDLE _CreateToolhelp32Snapshot(DWORD dwFlags, DWORD th32ProcessID)
{
	HANDLE hSnapShot = NULL;
	CreateToolhelp32Snapshot_Proc CreateToolhelp32Snapshot_Orig = NULL;
	CreateToolhelp32Snapshot_Orig = fpCreateToolhelp32Snapshot;

	if(g_bTest)
	{
		hSnapShot = CreateToolhelp32Snapshot_Orig(dwFlags, th32ProcessID);
		//hSnapShot = INVALID_HANDLE_VALUE;
		if (hSnapShot == INVALID_HANDLE_VALUE)
		{
			std::cout << "hSnapShot is INVALID_HANDLE_VALUE" << std::endl;
			return INVALID_HANDLE_VALUE;
		}
	}
	
	std::cout << "[6] Hooking Result CreateToolhelp32Snapshot" << std::endl;
	hSnapShot = CreateToolhelp32Snapshot_Orig(dwFlags, th32ProcessID);

	return hSnapShot;
}
BOOL _Process32First(HANDLE hSnapshot, LPPROCESSENTRY32 lppe)
{
	BOOL bRtn = FALSE;
	Process32First_Proc Process32First_Orig = NULL;
	Process32First_Orig = fpProcess32First;

	if (g_bTest)
	{
		bRtn = Process32First_Orig(hSnapshot, lppe);
	}
	
	std::cout << "[6] Hooking Result Process32First" << std::endl;

	bRtn = Process32First_Orig(hSnapshot, lppe);
	
	return bRtn;
}
BOOL _Process32Next(HANDLE hSnapshot, LPPROCESSENTRY32 lppe)
{
	BOOL bRtn = FALSE;
	Process32Next_Proc Process32Next_Orig = NULL;
	Process32Next_Orig = fpProcess32Next;

	if (g_bTest)
	{
		bRtn = Process32Next_Orig(hSnapshot, lppe);
	}
	
	std::cout << "Process ID = " << lppe->th32ParentProcessID << ", Process Name = " << lppe->szExeFile << std::endl;

	bRtn = Process32Next_Orig(hSnapshot, lppe);

	return bRtn;
}

HANDLE _CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	HANDLE hHandle = NULL;
	CreateFileA_Proc CreateFileA_Proc_Orig = NULL;
	CreateFileA_Proc_Orig = fpCreateFileA;

	lpFileName = "c:\\Test.ini";

	if (g_bTest)
	{
		hHandle = CreateFileA_Proc_Orig(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	}

	hHandle = CreateFileA_Proc_Orig(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);

	return hHandle;
}

BOOL _WritePrivateProfileString(LPCWSTR lpAppName, LPCWSTR lpKeyName, LPCWSTR lpString, LPCWSTR lpFileName)
{
	BOOL bRtn = FALSE;
	WritePrivateProfileString_Proc WritePrivateProfileString_Orig = NULL;
	WritePrivateProfileString_Orig = fpWritePrivateProfileString;

	if (g_bTest)
	{
		bRtn = WritePrivateProfileString_Orig(lpAppName, lpKeyName, lpString, lpFileName);
	}

	bRtn = WritePrivateProfileString_Orig(lpAppName, lpKeyName, lpString, lpFileName);

	return bRtn;
}

BOOL _GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName,	LPCTSTR lpDefault, LPTSTR lpReturnedString, DWORD nSize,LPCTSTR lpFileName)
{
	BOOL bRtn	 = FALSE;
	LPCTSTR temp = NULL;
	GetPrivateProfileString_Proc GetPrivateProfileString_Orig = NULL;
	GetPrivateProfileString_Orig = fpGetPrivateProfileString;

	// swap A,B
	if (TRUE == g_PrivateProfile_Flag)
	{
		
	}
	else
	{
		temp = lpAppName;
		lpAppName = lpKeyName;
		lpKeyName = temp;
	}
	
	if (g_bTest)
	{
		bRtn = GetPrivateProfileString_Orig(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);
	}

	bRtn = GetPrivateProfileString_Orig(lpAppName, lpKeyName, lpDefault, lpReturnedString, nSize, lpFileName);

	return bRtn;
}

UINT _GetPrivateProfileInt(LPCSTR lpAppName, LPCSTR lpKeyName, INT nDefault, LPCSTR lpFileName)
{
	UINT bRtn = 0;
	GetPrivateProfileInt_Proc GetPrivateProfileInt_Orig = NULL;
	GetPrivateProfileInt_Orig = fpGetPrivateProfileInt;

	// swap A,B
	//temp = lpAppName;
	//lpAppName = lpKeyName;
	//lpKeyName = temp;
	
	//Set Flag = true
	g_PrivateProfile_Flag = TRUE;

	if (g_bTest)
	{
		bRtn = GetPrivateProfileInt_Orig(lpAppName, lpKeyName, nDefault, lpFileName);
	}

	bRtn = GetPrivateProfileInt_Orig(lpAppName, lpKeyName, nDefault, lpFileName);

	return bRtn;
}

typedef struct _HOOKSETDATA
{
	CHAR DllName[32];
	CHAR ModuleName[64];
	PVOID MyPrcAddr;
	PVOID MyOrgPrcAddr;
} HOOKSETDATA;

HINSTANCE	g_hModule = NULL;
HHOOK		g_hHook = NULL;

//#define PrintAPIHookList_Count 3

HOOKSETDATA PrintAPIHookList[] =
{
	{"USER32.DLL",		"MessageBoxA",					_MessageBoxA,						&fpMessageBox},
	{"USER32.DLL",		"CharNextA",					_CharNextA,							&fpCharNextA},
	{"KERNEL32.DLL",	"GetLocalTime",					_GetLocalTime,						&fpGetLocalTime},
	{"KERNEL32.DLL",	"GetSystemTimes",				_GetSystemTimes,					&fpGetSystemTimes},
	{"KERNEL32.DLL",	"GetFullPathNameA",				_GetFullPathNameA,					&fpGetFullPathNameA},
	{"KERNEL32.DLL",	"CreateToolhelp32Snapshot",		_CreateToolhelp32Snapshot,			&fpCreateToolhelp32Snapshot},
	{"KERNEL32.DLL",	"Process32First",				_Process32First,					&fpProcess32First},
	{"KERNEL32.DLL",	"Process32Next",				_Process32Next,						&fpProcess32Next},
	{"KERNEL32.DLL",	"CreateFileA",					_CreateFileA,						&fpCreateFileA},
	{"KERNEL32.DLL",	"WritePrivateProfileStringA",	_WritePrivateProfileString,			&fpWritePrivateProfileString},
	{"KERNEL32.DLL",	"GetPrivateProfileStringA",		_GetPrivateProfileString,			&fpGetPrivateProfileString},
	{"KERNEL32.DLL",	"GetPrivateProfileIntA",		_GetPrivateProfileInt,				&fpGetPrivateProfileInt}
};

DWORD SetMiniHookAPI(HOOKSETDATA *pHooksetData);

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		g_hModule = (HINSTANCE)hinstDLL;
		
		MH_STATUS InitState = MH_OK;
		InitState = MH_Initialize();
		
		for (int i = 0; i < sizeof(PrintAPIHookList) / sizeof(HOOKSETDATA); i++)
		{
			SetMiniHookAPI(&PrintAPIHookList[i]);
		}
		
		//Hook 사용 요청 
		MH_EnableHook(MH_ALL_HOOKS);
		
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		std::cout << " DLL Detach " << std::endl;
		break;
	}
	default:
	{
		break;
	}
	}
	return TRUE;
}

LRESULT CALLBACK CBTFunc(INT nCode, WPARAM wParam, LPARAM lParam)
{
	return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

#ifdef __cplusplus
extern "C" {
#endif 
	__declspec(dllexport) BOOL HookStart()
	{
		g_hHook = SetWindowsHookEx(WH_CBT, CBTFunc, g_hModule, 0);

		if (g_hHook == NULL)
		{
			std::cout << " start error " << std::endl;
			return FALSE;
		}

		return TRUE;
	}
	__declspec(dllexport) BOOL HookStop()
	{
		BOOL	bReturn = FALSE;

		if (INVALID_HANDLE_VALUE != g_hHook)
		{
			bReturn = UnhookWindowsHookEx(g_hHook);
		}

		if (bReturn == FALSE)
		{
			std::cout << " Stop error " << std::endl;
			return FALSE;
		}
		
		MH_Uninitialize();

		return TRUE;
	}
#ifdef __cplusplus
}
#endif
DWORD SetMiniHookAPI(HOOKSETDATA *pHooksetData)
{
	HMODULE		hModule = NULL;
	LPVOID		funcAddr = NULL;
	MH_STATUS	mh_status = MH_OK;

	if (NULL == (hModule = LoadLibraryA(pHooksetData->DllName)))
	{
		std::cout << " LoadLibraryA is Null Pointer " << pHooksetData->DllName << std::endl;
		return ERROR;
	}

	if (NULL == (funcAddr = GetProcAddress(hModule, pHooksetData->ModuleName)))
	{
		std::cout << " GetProcAddress is Null Pointer " << pHooksetData->ModuleName << std::endl;
		return ERROR;
	}

	mh_status = MH_CreateHook(funcAddr, pHooksetData->MyPrcAddr, reinterpret_cast<void**>(pHooksetData->MyOrgPrcAddr));
	
	if(mh_status != MH_OK)
	{
		std::cout << "MH_CreateHook is Not OK" << mh_status << std::endl;
		return ERROR;
	}

	FreeLibrary(hModule);
	hModule = NULL;
	funcAddr = NULL;
	return TRUE;
}
