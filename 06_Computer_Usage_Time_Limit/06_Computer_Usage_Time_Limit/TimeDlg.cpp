// TimeDlg.cpp: 구현 파일
//

#include "pch.h"
#include "06_Computer_Usage_Time_Limit.h"
#include "TimeDlg.h"
#include "afxdialogex.h"


// TimeDlg 대화 상자

IMPLEMENT_DYNAMIC(TimeDlg, CDialogEx)

TimeDlg::TimeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TIME, pParent)
{

}

TimeDlg::~TimeDlg()
{
}

void TimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SHOW, m_ShowTime);
	DDX_Control(pDX, IDC_STATIC_END, m_EndTime);
	DDX_Control(pDX, IDC_RICHEDIT_GUIDE_TIME, m_rcGuide3);
}

BEGIN_MESSAGE_MAP(TimeDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL TimeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	theApp.m_hWndTime = GetSafeHwnd();

	// Dialog 이름
	SetWindowText(_T("Time"));

	// 폰트설정 
	CFont cf;
	cf.CreatePointFont(100, _T("굴림"));
	//m_ShowTime.SetFont(&cf, TRUE);
	GetDlgItem(IDC_STATIC_SHOW)->SetFont(&cf);

	CFont cf2;
	cf2.CreatePointFont(5, _T("굴림"));
	GetDlgItem(IDC_STATIC_END)->SetFont(&cf2);
	//m_EndTime.SetFont(&cf2, TRUE);

	// 시간설정
	CString cstrShowTimeHour = CTime::GetCurrentTime().Format("%H");
	CString cstrShowTimeMin  = CTime::GetCurrentTime().Format("%M");
	CString cstrShow;
	cstrShow.Format(_T("현재시간 : %d 시 %d 분"), _ttoi(cstrShowTimeHour), _ttoi(cstrShowTimeMin));
	m_ShowTime.SetWindowText(cstrShow);

	CString cstrEndTime = theApp.m_EndTime;
	INT nTempMin = _ttoi(cstrEndTime.Right(2));
	INT nTempHour = _ttoi(cstrEndTime.Left(2));
	cstrEndTime.Format(_T("종료시간 : %d시 %d분"), nTempHour, nTempMin);
	m_EndTime.SetWindowText(cstrEndTime);

	GuideMsg3(_T("문의: post4204@naver.com \r\n"), RGB(0, 0, 255));

	return TRUE;
}

void TimeDlg::GuideMsg3(LPCTSTR strText, COLORREF TextColor)
{
	int first_pos = m_rcGuide3.LineIndex(m_rcGuide3.GetLineCount());
	m_rcGuide3.SetSel(first_pos, first_pos);
	CPoint point;
	point = m_rcGuide3.PosFromChar(first_pos);
	m_rcGuide3.SetCaretPos(point);
	m_rcGuide3.SetFocus();

	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = TextColor;

	m_rcGuide3.SetBackgroundColor(0, RGB(240, 240, 240));

	m_rcGuide3.SetSelectionCharFormat(cf);
	m_rcGuide3.ReplaceSel(strText);
}
