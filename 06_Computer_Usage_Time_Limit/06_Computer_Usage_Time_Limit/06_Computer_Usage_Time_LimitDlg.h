#pragma once
#include "md5.h"
#include "CloseCheckDlg.h"
#include "TimeDlg.h"
#define WM_TRAY_NOTIFYCATION WM_APP + 10

class CMy06ComputerUsageTimeLimitDlg : public CDialogEx
{
public:
	CMy06ComputerUsageTimeLimitDlg(CWnd* pParent = nullptr);	
	CloseCheckDlg* m_CloseCheckDlg;
	TimeDlg* m_TimeDlg;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY06_COMPUTER_USAGE_TIME_LIMIT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	afx_msg void OnCancelOverride();

	static void fnTimeThread(void* tp);

	LRESULT OnTrayAction(WPARAM wParam, LPARAM lParam);
	void OnShellClose();
	void OnShellOpen();
	void OnShellTime();
	void GuideMsg(LPCTSTR strText, COLORREF TextColor);

	void openShell();

	CRichEditCtrl m_Rich_GuideMessage;
	CButton m_btStart;
	CEdit m_Edit_Time;
	CEdit m_Edit_PW;

	CString m_cTime;
	CString m_EndHour;
	CString m_EndMin;
};