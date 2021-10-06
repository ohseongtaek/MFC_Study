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
	//CFont cf;
	//cf.CreatePointFont(100, _T("굴림"));
	//m_ShowTime.SetFont(&cf, TRUE);
	//GetDlgItem(IDC_STATIC_SHOW)->SetFont(&cf);

	//CFont cf2;
	//cf2.CreatePointFont(5, _T("굴림"));
	//GetDlgItem(IDC_STATIC_END)->SetFont(&cf2);
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
	cstrEndTime.Format(_T("종료시간 : %d 시 %d 분"), nTempHour, nTempMin);
	m_EndTime.SetWindowText(cstrEndTime);

	return TRUE;
}