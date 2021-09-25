#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.


class CMy06ComputerUsageTimeLimitApp : public CWinApp
{
public:
	CMy06ComputerUsageTimeLimitApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnShellClose();
	afx_msg void OnShellOpen();
};

extern CMy06ComputerUsageTimeLimitApp theApp;
