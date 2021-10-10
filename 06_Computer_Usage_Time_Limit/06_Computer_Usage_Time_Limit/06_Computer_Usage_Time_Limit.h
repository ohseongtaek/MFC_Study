#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include <string>

class CMy06ComputerUsageTimeLimitApp : public CWinApp
{
public:
	CMy06ComputerUsageTimeLimitApp();

public:
	virtual BOOL InitInstance();
	CString m_cstrInputPASS;
	HWND m_hWndMain;
	HWND m_hWndTime;
	std::string strToMD5(CString pw);
	BOOL m_bLogin;
	CString m_EndTime;
	BOOL m_bThreadClose;

	DECLARE_MESSAGE_MAP()
};

extern CMy06ComputerUsageTimeLimitApp theApp;
