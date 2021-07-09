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

	// ��ü ���μ��� ����� Ȯ���ϴ� API 
	pe.dwSize = sizeof(PROCESSENTRY32);	
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	// ������ ���鼭 notepad.exe���ϰ� �������� �ִ��� Ȯ�� 
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

	// ���� �޸� �Ҵ� 
	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

	// ���� �޸� ������ �ۼ� �� dll �̸� �ֱ� 
	WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllName, dwBufSize, NULL);

	// KERNEL32.DLL �� �׻� ����Ǵ� DLL
	// ���� LoadLibraryA API �ּҸ� ��´�.
	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA");

	// ������ ���� API �ּҸ� �̿��� notepad.exe ���Ͽ��� thread �� �����Ѵ�.
	// ������ ������ ���� dll �� �����ϸ� iexplore ���� naver â�� ������.
	hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);

	CloseHandle(hThread);
	CloseHandle(hProcess);

	return TRUE;
}