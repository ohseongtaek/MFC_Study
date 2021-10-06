﻿#include "pch.h"
#include "framework.h"
#include "06_Computer_Usage_Time_Limit.h"
#include "06_Computer_Usage_Time_LimitDlg.h"
#include "afxdialogex.h"
#include "md5.h"
#include <strsafe.h>
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// User Function 
std::string strToMD5(CString pw);
// End


CMy06ComputerUsageTimeLimitDlg::CMy06ComputerUsageTimeLimitDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY06_COMPUTER_USAGE_TIME_LIMIT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy06ComputerUsageTimeLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDITGUIDEMESSAGE, m_Rich_GuideMessage);
	DDX_Control(pDX, IDC_EDIT1, m_Edit_Time);
	DDX_Control(pDX, IDC_EDIT2, m_Edit_PW);
	DDX_Control(pDX, IDOK, m_btStart);
}

BEGIN_MESSAGE_MAP(CMy06ComputerUsageTimeLimitDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_TRAY_NOTIFYCATION, &CMy06ComputerUsageTimeLimitDlg::OnTrayAction)
	ON_COMMAND(Shell_Close, &CMy06ComputerUsageTimeLimitDlg::OnShellClose)
	ON_COMMAND(Shell_Open, &CMy06ComputerUsageTimeLimitDlg::OnShellOpen)
	ON_COMMAND(Shell_TimeCheck, &CMy06ComputerUsageTimeLimitDlg::OnShellTime)
	ON_BN_CLICKED(IDOK, &CMy06ComputerUsageTimeLimitDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMy06ComputerUsageTimeLimitDlg 메시지 처리기

BOOL CMy06ComputerUsageTimeLimitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);		// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, TRUE);		// 작은 아이콘을 설정합니다.

	//seongtaek10.oh 시간 다이얼로그만 구현하면 끝 
	SetWindowText(_T("Computer Time Limit made by seongtaekoh"));

	// Tray 추가 Start 
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hWnd = m_hWnd;
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	nid.uCallbackMessage = WM_TRAY_NOTIFYCATION;
	lstrcpy(nid.szTip, _T("Computer Time Limit"));
	::Shell_NotifyIcon(NIM_ADD, &nid);
	// Tray 추가 End 

	// Dialog 중앙 배치 Start
	CenterWindow();
	// Dialog 중앙 배치 End

	// 안내 메시지 출력 Start
	//m_Rich_GuideMessage.MoveWindow(12, 270, 400, 65, FALSE);
	GuideMsg(_T("1.시간입력(분) : 사용시간은 분 단위 입니다. ex)60\r\n"), RGB(0, 0, 0));
	GuideMsg(_T("2.패스워드     : 사용중에 프로그램을 종료하기 위함 입니다.\r\n"), RGB(0, 0, 0));
	GuideMsg(_T("※컴퓨터 이용시간 초과 시 바로 종료됩니다.\r\n"), RGB(255, 0, 0));
	GuideMsg(_T("문의: post4204@naver.com \r\n"), RGB(0, 0, 255));
	m_Rich_GuideMessage.SetReadOnly(TRUE);
	// 안내 메시지 출력 End 

	theApp.m_hWndMain = GetSafeHwnd();

	ShowWindow(SW_SHOW);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMy06ComputerUsageTimeLimitDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

HCURSOR CMy06ComputerUsageTimeLimitDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy06ComputerUsageTimeLimitDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	//Dialog x button 누를 경우 
	if (nID == SC_CLOSE)
	{
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
	}
}

LRESULT CMy06ComputerUsageTimeLimitDlg::OnTrayAction(WPARAM wParam, LPARAM lParam)
{
	switch (lParam)
	{
		case WM_RBUTTONDOWN:
		{
			CPoint ptMouse;
			::GetCursorPos(&ptMouse);
			CMenu  menu;
			menu.LoadMenu(IDR_MENU1);
			CMenu* pMenu = menu.GetSubMenu(0);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, ptMouse.x, ptMouse.y, AfxGetMainWnd());
			break;
		}
		case WM_LBUTTONDBLCLK:
		{
			openShell();
			break;
		}
	}
	return 1;
}

void CMy06ComputerUsageTimeLimitDlg::OnShellClose()
{
	m_CloseCheckDlg = new CloseCheckDlg;
	m_CloseCheckDlg->Create(IDD_DIALOG_CLOSE, this);
	m_CloseCheckDlg->ShowWindow(SW_SHOW);
}

void CMy06ComputerUsageTimeLimitDlg::OnShellOpen()
{
	openShell();
}

void CMy06ComputerUsageTimeLimitDlg::OnShellTime()
{
	m_TimeDlg = new TimeDlg;
	m_TimeDlg->Create(IDD_DIALOG_TIME, this);
	m_TimeDlg->ShowWindow(SW_SHOW);
}

void CMy06ComputerUsageTimeLimitDlg::GuideMsg(LPCTSTR strText, COLORREF TextColor)
{
	int first_pos = m_Rich_GuideMessage.LineIndex(m_Rich_GuideMessage.GetLineCount());
	m_Rich_GuideMessage.SetSel(first_pos, first_pos);
	CPoint point;
	point = m_Rich_GuideMessage.PosFromChar(first_pos);
	m_Rich_GuideMessage.SetCaretPos(point);
	m_Rich_GuideMessage.SetFocus();

	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = TextColor;

	m_Rich_GuideMessage.SetBackgroundColor(0,RGB(240, 240, 240));

	m_Rich_GuideMessage.SetSelectionCharFormat(cf);
	m_Rich_GuideMessage.ReplaceSel(strText);
}

void CMy06ComputerUsageTimeLimitDlg::OnBnClickedOk()
{
	CString ctime;
	CString pw;
	std::string strMD5pw;
	INT nHour;
	INT nMin;

	//시간값 얻기 
	m_Edit_Time.GetWindowText(ctime);
	INT nInputTime = _ttoi(ctime);

	//비밀번호 얻기 
	m_Edit_PW.GetWindowText(pw);

	// 값 확인하기 
	if (ctime.IsEmpty() || pw.IsEmpty())
	{
		//::AfxMessageBox(_T("시간과 패스워드를 입력해주세요"));
		MessageBox(_T("시간과 패스워드를 입력해주세요"), _T("Info"), MB_ICONASTERISK);

		//로그인 상태 체크 
		theApp.m_bLogin = FALSE;
		return;
	}

	// 패스워드 MD5변환 하기 
	strMD5pw = theApp.strToMD5(pw);
	theApp.m_cstrInputPASS = strMD5pw.c_str();

	// 시간값 계산하기 
	CString cstrTimeHour = CTime::GetCurrentTime().Format("%H");
	CString cstrTimeMin	 = CTime::GetCurrentTime().Format("%M");
	INT nTempMin  = _ttoi(cstrTimeMin);
	INT nTempHour = _ttoi(cstrTimeHour);

	INT nTempTotalMin = nTempMin + nInputTime;
	INT nUpHour = nTempTotalMin / 60;

	if (nTempHour + nUpHour >= 24)
	{
		nHour = nTempHour + nUpHour;
		nHour = nHour - 24;
		nMin = nTempTotalMin - (nUpHour * 60);
	}
	else
	{
		nHour = nTempHour + nUpHour;
		nMin = nTempTotalMin - (nUpHour * 60);
	}

	char szToStr[MAX_PATH] = { 0, };
	StringCbPrintfA(szToStr, sizeof(szToStr), "%d%d", nHour, nMin);
	std::string strTime(szToStr);
	theApp.m_EndTime = strTime.c_str();
	
	m_EndHour.Format(_T("%d"), nHour);
	m_EndMin.Format(_T("%d"), nMin);
	
	//로그인 상태 체크
	theApp.m_bLogin = TRUE;

	//시작 시점 
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
	
	// 새로운 다이얼로그 생성 해야함 (시간계산해주는것)
	m_TimeDlg = new TimeDlg;
	m_TimeDlg->Create(IDD_DIALOG_TIME, this);
	m_TimeDlg->ShowWindow(SW_SHOW);

	// thread 시작 
	HANDLE hLogThread = (HANDLE)_beginthread(fnTimeThread, 0, (void*)this);
	
}

void CMy06ComputerUsageTimeLimitDlg::fnTimeThread(void* tp)
{
	//::AfxMessageBox(_T("seongtaek"));
	CMy06ComputerUsageTimeLimitDlg* pTimeCls = (CMy06ComputerUsageTimeLimitDlg*)tp;
	TimeDlg* pTimeCls2 = (TimeDlg*)tp;
	
	CString cstrShowTimeHour = 0;
	CString cstrShowTimeMin = 0;
	INT nTempMin = 0;
	INT nTempHour = 0;

	while (1)
	{
		BOOL bUpdate = TRUE;
		BOOL bUpdate_TIME = TRUE;

		cstrShowTimeHour = CTime::GetCurrentTime().Format("%H");
		cstrShowTimeMin  = CTime::GetCurrentTime().Format("%M");

		if (nTempHour >= _ttoi(pTimeCls->m_EndHour) && nTempMin >= _ttoi(pTimeCls->m_EndMin))
		{
			bUpdate = FALSE;
		}

		if (nTempHour == _ttoi(cstrShowTimeHour) && nTempMin == _ttoi(cstrShowTimeMin))
		{
			bUpdate_TIME = FALSE;
		}
		
		nTempHour = _ttoi(cstrShowTimeHour);
		nTempMin = _ttoi(cstrShowTimeMin);

		if (bUpdate && bUpdate_TIME)
		{
			CString cstrText;
			cstrText.Format(_T("현재시간 : %d 시 %d 분"), nTempHour, nTempMin);
			pTimeCls->m_TimeDlg->m_ShowTime.SetWindowText(cstrText);
		}
		else
		{
			if (!bUpdate)
			{
				::AfxMessageBox(_T("컴퓨터를 종료합니다 (이 타이밍에 종료하면됨)"));
				//seongtaek10.oh 
				//강제종료 커맨드 입력하기 
				//메뉴에서 시간창 open 확인하기 
				break;
			}
			else if (bUpdate_TIME)
			{
				// do not anything
			}
		}
		Sleep(10);
	}
}

void CMy06ComputerUsageTimeLimitDlg::openShell()
{
	// 로그인 상태에 따라 화면 변화 
	if (theApp.m_bLogin == TRUE)
	{
		// 가이드 메시지 변경 
		m_Rich_GuideMessage.SetSel(0, -1);
		m_Rich_GuideMessage.Clear();
		m_Rich_GuideMessage.SetWindowText(_T(""));

		GuideMsg(_T("이미 설정된 시간이 있습니다. \r\n"), RGB(255, 0, 0));
		GuideMsg(_T("재설정 시 프로그램 종료 후 다시 시작해주세요. \r\n"), RGB(0, 0, 0));
		GuideMsg(_T("문의: post4204@naver.com \r\n"), RGB(0, 0, 255));

		// 버튼 동작 변경 
		m_Edit_Time.SetReadOnly(TRUE);
		m_Edit_PW.SetReadOnly(TRUE);
		m_btStart.EnableWindow(FALSE);
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
	}
}