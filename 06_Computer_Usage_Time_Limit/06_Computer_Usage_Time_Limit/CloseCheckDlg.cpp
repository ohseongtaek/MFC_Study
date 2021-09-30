// CloseCheckDlg.cpp: 구현 파일
//

#include "pch.h"
#include "06_Computer_Usage_Time_Limit.h"
#include "CloseCheckDlg.h"
#include "afxdialogex.h"
#include <string>

// CloseCheckDlg 대화 상자

IMPLEMENT_DYNAMIC(CloseCheckDlg, CDialogEx)

CloseCheckDlg::CloseCheckDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CLOSE, pParent)
{

}

CloseCheckDlg::~CloseCheckDlg()
{
}

void CloseCheckDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT_CLOSE_MSG, m_reCloseMsg);
	DDX_Control(pDX, IDC_EDIT_CLOSE_PW, m_edClosePW);
}

BEGIN_MESSAGE_MAP(CloseCheckDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CloseCheckDlg::OnBnClickedButtonClose)
	//ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CloseCheckDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(_T("비밀번호 입력"));

	// 비밀번호 입력 후 상태 
	m_bStatus = FALSE;
	
	GuideMsg2(_T("시작 시 입력한 비밀번호를 입력해주세요. \r\n"), RGB(0, 0, 0));
	GuideMsg2(_T("문의: post4204@naver.com \r\n"), RGB(0, 0, 255));

	return TRUE;
}

void CloseCheckDlg::GuideMsg2(LPCTSTR strText, COLORREF TextColor)
{
	int first_pos = m_reCloseMsg.LineIndex(m_reCloseMsg.GetLineCount());
	m_reCloseMsg.SetSel(first_pos, first_pos);
	CPoint point;
	point = m_reCloseMsg.PosFromChar(first_pos);
	m_reCloseMsg.SetCaretPos(point);
	m_reCloseMsg.SetFocus();

	CHARFORMAT cf;
	memset(&cf, 0, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = TextColor;

	m_reCloseMsg.SetBackgroundColor(0, RGB(240, 240, 240));

	m_reCloseMsg.SetSelectionCharFormat(cf);
	m_reCloseMsg.ReplaceSel(strText);
}

void CloseCheckDlg::OnBnClickedButtonClose()
{
	CString InputPW;
	CString FinalDiff;
	std::string strMD5pw;
	m_edClosePW.GetWindowText(InputPW);

	if (InputPW.IsEmpty())
	{
		//::AfxMessageBox(_T("시간과 패스워드를 입력해주세요"));
		MessageBox(_T("패스워드를 입력해주세요"), _T("Info"), MB_ICONASTERISK);
		return;
	}

	// 패스워드 MD5변환 하기 
	strMD5pw = theApp.strToMD5(InputPW);
	FinalDiff = strMD5pw.c_str();

	if (!theApp.m_cstrInputPASS.Compare(FinalDiff))
	{
		MessageBox(_T("성공적으로 종료하겠습니다."), _T("Info"), MB_ICONASTERISK);

		// 입력창 다이얼로그 종료 
		::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);

		// 트레이 종료 
		NOTIFYICONDATA nid;
		ZeroMemory(&nid, sizeof(nid));
		nid.cbSize = sizeof(nid);
		nid.uID = 0;
		nid.hWnd = theApp.m_hWndMain;

		BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &nid);
		if (!bRet)
		{
			MessageBox(_T("종료 실패 \r\n 다시 종료해주세요"), _T("Error"), MB_ICONERROR);
		}
		
		// 메인 윈도우와 함께 프로그램전체 종료 
		::SendMessage(theApp.m_hWndMain, WM_CLOSE, NULL, NULL);
	}
	else
	{
		MessageBox(_T("비밀번호가 틀렸습니다."), _T("Error"), MB_ICONERROR);
	}
}