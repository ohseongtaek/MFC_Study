#include "pch.h"
#include "framework.h"
#include "06_Computer_Usage_Time_Limit.h"
#include "06_Computer_Usage_Time_LimitDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMy06ComputerUsageTimeLimitDlg::CMy06ComputerUsageTimeLimitDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY06_COMPUTER_USAGE_TIME_LIMIT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy06ComputerUsageTimeLimitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDITGUIDEMESSAGE, m_Rich_GuideMessage);
}

BEGIN_MESSAGE_MAP(CMy06ComputerUsageTimeLimitDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_MESSAGE(WM_TRAY_NOTIFYCATION, &CMy06ComputerUsageTimeLimitDlg::OnTrayAction)
	ON_COMMAND(Shell_Close, &CMy06ComputerUsageTimeLimitDlg::OnShellClose)
	ON_COMMAND(Shell_Open, &CMy06ComputerUsageTimeLimitDlg::OnShellOpen)
	ON_BN_CLICKED(IDOK, &CMy06ComputerUsageTimeLimitDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CMy06ComputerUsageTimeLimitDlg 메시지 처리기

BOOL CMy06ComputerUsageTimeLimitDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);		// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, TRUE);		// 작은 아이콘을 설정합니다.

	SetWindowText("Computer Time Limit made by seongtaekoh");

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
	GuideMsg("컴퓨터 이용시간 초과 시 바로 종료됩니다.\r\n", RGB(0, 0, 0));
	GuideMsg("문의: post4204@naver.com\r\n", RGB(0, 0, 255));
	// 안내 메시지 출력 End 

	// seongtaekoh 각종 버튼 구현 

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
	if (nID == SC_CLOSE)
	{
		AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
	}
	//CDialogEx::OnSysCommand(nID, lParam);
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
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
			break;
		}
	}
	return 1;
}

void CMy06ComputerUsageTimeLimitDlg::OnShellClose()
{
	// seongtaekoh 아무나 종료하지 못하도록 설정 해야함 //
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.uID = 0;
	nid.hWnd = GetSafeHwnd();

	BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &nid);
	if (!bRet)
	{
		// seongtaekoh log 로 변경하기 
		::AfxMessageBox(_T("Tray Remove Fail"));
	}
	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}

void CMy06ComputerUsageTimeLimitDlg::OnShellOpen()
{
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
}

void CMy06ComputerUsageTimeLimitDlg::OnBnClickedOk()
{
	//seongtaekoh 하이드 전에 실제 저장된 정보를 저장해서 ini파일에 저장하기 
	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
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
