// dllmain.cpp : DLL 애플리케이션의 진입점을 정의합니다.
#include "pch.h"
#include <iostream>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
		case DLL_PROCESS_ATTACH:
		{
			std::cout << "[1] DLL Attach " << std::endl;
			break;
		}
			
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
			break;
		default:
			break;
    }
    return TRUE;
}

#ifdef __cplusplus
extern "C" {
#endif 
	__declspec(dllexport) void seongtaekTemp()
	{
		std::cout << "[4] externC " << std::endl;
		std::cout << "[5] externC " << std::endl;
	}
#ifdef __cplusplus
}
#endif
	