#include "windows.h"
#include <conio.h>
#include <iostream>
#include "stdio.h"
#include "tlhelp32.h"

#define DEF_DLL_NAME  "D:\\seongtaek_TestCode\\222\\x64\\Release\\222.dll"
#define SEONGTAEK_TEMP "seongtaekTemp"
typedef void(*SEONGTAEK_TIMING)();

int main()
{
	HMODULE		  hModule = NULL;
	SEONGTAEK_TIMING TIMING = NULL;

	std::cout << "[0]LoadLibraryA START" << std::endl;
	hModule = LoadLibraryA(DEF_DLL_NAME);
	
	if (hModule == NULL)
	{
		return ERROR;
	}

	std::cout << "[2]LoadLibraryA End " << std::endl;
	TIMING = (SEONGTAEK_TIMING)GetProcAddress(hModule, SEONGTAEK_TEMP);
	
	std::cout << "[3]TIMING() START" << std::endl;
	TIMING();
	std::cout << "[6]TIMING() START" << std::endl;

	Sleep(10000);
	return 0;
}
