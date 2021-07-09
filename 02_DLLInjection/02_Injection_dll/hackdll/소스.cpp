#include "stdio.h"
#include "windows.h"

#define DEF_CMD			"C:\\Program Files\\Internet Explorer\\iexplore.exe"
#define DEF_ADDR		"www.naver.com"

DWORD WINAPI ThreadProc(LPVOID lParam)
{
	char				szCmd[MAX_PATH] = { 0, };
	STARTUPINFO			si = { 0, };
	PROCESS_INFORMATION pi = { 0, };
	
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;

	wsprintf(szCmd, "%s %s", DEF_CMD, DEF_ADDR);

	CreateProcess(NULL, (LPTSTR)(LPCTSTR)szCmd, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	HANDLE hThread = NULL;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		hThread = CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
		CloseHandle(hThread);
		break;
	}

	return TRUE;
}

