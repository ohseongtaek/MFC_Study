#include "stdio.h"
#include "windows.h"
#include "tlhelp32.h"

#define DEF_PROC_NAME	("notepad.exe")
#define DEF_DLL_PATH	("D:\\seongtaek_TestCode\\02_Injection_dll\\x64\\Release\\hackdll.dll")

DWORD FindProcessID(LPCTSTR szProcessName);
BOOL InjectDll(DWORD dwPID, LPCTSTR szDllName);

int main(int argc, char* argv[])
{
	DWORD dwPID = 0xFFFFFFFF;

	// find process
	dwPID = FindProcessID(DEF_PROC_NAME);
	if (dwPID == 0xFFFFFFFF)
	{
		printf("There is no <%s> process!\n", DEF_PROC_NAME);
		return 1;
	}

	// inject dll
	InjectDll(dwPID, DEF_DLL_PATH);

	return 0;
}

DWORD FindProcessID(LPCTSTR szProcessName)
{
	DWORD dwPID = 0xFFFFFFFF;
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;

	// 전체 프로세스 목록을 확인하는 API 
	pe.dwSize = sizeof(PROCESSENTRY32);	
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	// 루프를 돌면서 notepad.exe파일과 같은것이 있는지 확인 
	Process32First(hSnapShot, &pe);
	do
	{
		if (!_stricmp(szProcessName, pe.szExeFile))
		{
			dwPID = pe.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapShot, &pe));

	CloseHandle(hSnapShot);

	return dwPID;
}

BOOL InjectDll(DWORD dwPID, LPCTSTR szDllName)
{
	HANDLE hProcess, hThread;
	LPVOID pRemoteBuf;
	DWORD dwBufSize = lstrlen(szDllName) + 1;
	LPTHREAD_START_ROUTINE pThreadProc;

	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)))
	{
		return FALSE;
	}

	// 가상 메모리 할당 
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	// 가상 메모리 여유분 작성 및 dll 이름 넣기 
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllName, dwBufSize, NULL);

	// KERNEL32.DLL 은 항상 실행되는 DLL
	// 따라서 LoadLibraryA API 주소를 얻는다.
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	// 위에서 얻은 API 주소를 이용해 notepad.exe 파일에서 thread 로 실행한다.
	// 실행중 위에서 넣은 dll 을 실행하면 iexplore 에서 naver 창이 열린다.
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}