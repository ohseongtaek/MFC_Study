#include "windows.h"
#include <map>
#include <conio.h>
#include <iostream>
#include "stdio.h"
#include "tlhelp32.h"
#include "Shlwapi.h"
#include <io.h>

#define DEF_DLL_NAME	"D:\\seongtaek_TestCode\\05_Global_Hooking_Control\\x64\\Release\\05_Global_Hooking_Control.dll"
#define DEF_STR_INIFILE "C:\\Test.ini"
#define DEF_HOOKSTART	"HookStart"
#define DEF_HOOKSTOP	"HookStop"

typedef void(*PFN_HOOKSTART)();
typedef void(*PFN_HOOKSTOP)();
typedef void(*PFN_TEST)();

PFN_HOOKSTART HookStart = NULL;
PFN_HOOKSTOP  HookStop = NULL;
PFN_TEST	  Test = NULL;

typedef struct _INPUTOST {
	BOOL bMessageBox;
	BOOL bCharNext;
	BOOL bGetLocalTime;
	BOOL bGetSystemTimes;
	BOOL bGetFullPathName;
	BOOL bSnapAPI;
	BOOL bProcess32First;
	BOOL bProcess32Next;
	BOOL bCreateFile;
	BOOL bGetPrivateProfileString;
	BOOL bGetPrivateProfileInt;

		// Test하고싶은 API에 True False 설정하기 
		_INPUTOST()
		{
			bMessageBox				  = TRUE;
			bCharNext				  = FALSE;
			bGetLocalTime			  = FALSE;
			bGetSystemTimes			  = FALSE;
			bGetFullPathName		  = FALSE;
			bSnapAPI				  = FALSE;
			bCreateFile				  = FALSE;
			bGetPrivateProfileString  = FALSE;
			bGetPrivateProfileInt	  = FALSE;
		}
}INPUTOST;

int main()
{
	HMODULE  hModule	 = NULL;
	DWORD	 error_code  = 0;
	INPUTOST Temp;

	hModule = LoadLibraryA(DEF_DLL_NAME);
	if (hModule != NULL)
	{
		HookStart = (PFN_HOOKSTART)GetProcAddress(hModule, DEF_HOOKSTART);
		HookStop = (PFN_HOOKSTOP)GetProcAddress(hModule, DEF_HOOKSTOP);
	}
	else
	{
		std::cout << "what seongtaek" << std::endl;
	}

	if (HookStart)
	{
		HookStart();
	}
	
	// Input Test Code And Hooking 

	//MessageBoxA Hooking 
	if (FALSE != Temp.bMessageBox)
	{
		MessageBoxA(NULL, "Call!", "good", MB_OK);
	}
		
	//CharNextA Hooking 
	if (FALSE != Temp.bCharNext)
	{
		LPCSTR pszTest = "TEST";
		LPSTR temp = CharNextA(pszTest);
		std::cout << "[2] Hooking Result CharNextA [" << temp << "]" << std::endl;
	}

	//GetLocalTime Hooking 
	if (FALSE != Temp.bGetLocalTime)
	{
		SYSTEMTIME localTime;
		GetLocalTime(&localTime);
		std::cout << "[3] Hooking Result LocalTime [" << localTime.wYear << "년 " << localTime.wMonth << "월 " << localTime.wDay << "일 " << localTime.wHour << "시 " << localTime.wMinute << "분] " << std::endl;
	}
	
	//GetSystemTimes Hooking
	if (FALSE != Temp.bGetSystemTimes)
	{
		FILETIME idle;
		FILETIME kernelTime;
		FILETIME userTime;
		GetSystemTimes(&idle, &kernelTime, &userTime);
		std::cout << "[4] Hooking Result GetSystemTimes [Idle High = " << idle.dwHighDateTime << ", Idle Low = " << idle.dwLowDateTime << "]"
													<< "[Kernel High = " << kernelTime.dwHighDateTime << ", Kernel Low = " << kernelTime.dwLowDateTime << "]"
													<< "[User High = " << userTime.dwHighDateTime << ", User Low = " << userTime.dwLowDateTime << "]" << std::endl;
	}
	
	//GetFullPathName Hooking 
	if (FALSE != Temp.bGetFullPathName)
	{
		LPCSTR lpFileName = "seongtaek_NotePad";
		TCHAR lpBuffer[MAX_PATH] = "";
		TCHAR** lppart = NULL;
		GetFullPathNameA(lpFileName, MAX_PATH, lpBuffer, lppart);
		std::cout << "[5] Hooking Result GetFullPathNameA [" << lpBuffer << "]" << std::endl;
	}

	//CreateToolhelp32Snapshot
	//Process32First
	//Process32Next
	if (FALSE != Temp.bSnapAPI)
	{
		std::map<DWORD, TCHAR*> ProcessList;
		std::map<DWORD, TCHAR*>::iterator ProcessList_iter;

		HANDLE hSnapShot = NULL;
		BOOL bProcessFound = FALSE;
		PROCESSENTRY32 Process32Entry = { 0, };
		Process32Entry.dwSize = sizeof(PROCESSENTRY32);

		hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (INVALID_HANDLE_VALUE == hSnapShot) {
			std::cout << "Error 1 = hSnapShot is INVALID_HANDLE_VALUE" << std::endl;
		}

		bProcessFound = Process32First(hSnapShot, &Process32Entry);
		if (bProcessFound == FALSE)
		{
			std::cout << "Error 2 = Process32First is FALSE " << std::endl;
		}

		while (bProcessFound)
		{
			//std::cout << Process32Entry.szExeFile << std::endl;
			ProcessList.insert(std::make_pair(Process32Entry.th32ProcessID, Process32Entry.szExeFile));
			bProcessFound = Process32Next(hSnapShot, &Process32Entry);
		}
		CloseHandle(hSnapShot);

		std::cout << "[6] Hooking Result ProcessAPI" << ProcessList.size() << std::endl;
	}
	
	//CreateFile 
	if (TRUE == Temp.bCreateFile)
	{
		HANDLE hFile		= NULL;
		LPCSTR pszFilePath  = "c:\\test.txt";
		BOOL   bRtn			= FALSE;

		hFile = CreateFileA(pszFilePath, GENERIC_ALL, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		if (INVALID_HANDLE_VALUE == hFile)
		{
			std::cout << "[7] Hooking Result CreateFileA [CreateFileA is Null Handle]" << std::endl;
		}
		else
		{
			
			std::cout << "[7] Hooking Result CreateFileA [CreateFileA is Success]" << std::endl;
		}

		bRtn = _access(pszFilePath,0);

		if (TRUE == bRtn)
		{
			std::cout << "[7] Hooking Reuslt CreateFileA [CreateFileA is Hooking Fail]" << std::endl;
		}
		else
		{
			std::cout << "[7] Hooking Reuslt CreateFileA [CreateFileA is Hooking Success]" << std::endl;
		}	

		CloseHandle(hFile);
		hFile = NULL;
	}
	 
	//GetPrivateProfileString
	if (TRUE == Temp.bGetPrivateProfileString)
	{
		BOOL bRtn				= FALSE;
		DWORD dwRtn				= FALSE;
		TCHAR RStr[MAX_PATH]	= { 0, };
		LPCSTR pszSectionName	= "TEST";
		LPCSTR pszKeyName		= "SEONGTAEK";
		LPCSTR pszValueName		= "OH";
		INT iTemp				= 0;

		bRtn = WritePrivateProfileString(pszSectionName, pszKeyName, pszValueName, DEF_STR_INIFILE);
		if (TRUE == bRtn)
		{
			dwRtn = GetPrivateProfileString(pszKeyName, pszSectionName, "DEFAULT", RStr, MAX_PATH, DEF_STR_INIFILE);
		}
		else
		{
			int temp = GetLastError();
			std::cout << temp << std::endl;
		}

		std::cout << "[8] Hooking Result GetPrivateProfileString [Success = " << RStr << "]" << std::endl;
	}

	//GetPrivateProfileInt
	if (TRUE == Temp.bGetPrivateProfileInt)
	{
		BOOL bRtn				= TRUE;
		UINT dwRtn				= 0;
		LPCSTR pszSectionName	= "TEST2";
		LPCSTR pszKeyName		= "SEONGTAEK";
		LPCSTR pszValueName		= "960614";
		
		bRtn = WritePrivateProfileString(pszSectionName, pszKeyName, pszValueName, DEF_STR_INIFILE);
		if (TRUE == bRtn)
		{
			dwRtn = GetPrivateProfileInt(pszSectionName, pszKeyName, -1, DEF_STR_INIFILE);
		}
		
		std::cout << "[9] Hooking Result GetPrivateProfileInt [Success = " << dwRtn << "]" <<std::endl;
	}


	char temp[10] = "seong12";

	char ch;
	while (1) 
	{
		printf("문자 입력 (q,Q 입력시 프로그램 종료) : ");
		std::cin >> ch;
		getchar();
		if (ch == 'Q' || ch == 'q') {
			exit(1);
		}
	}

	return 0;
}