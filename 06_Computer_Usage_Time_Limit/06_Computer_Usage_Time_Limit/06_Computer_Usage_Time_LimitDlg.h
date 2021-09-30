#pragma once
#include "md5.h"
#include "CloseCheckDlg.h"
#define WM_TRAY_NOTIFYCATION WM_APP + 10

class CMy06ComputerUsageTimeLimitDlg : public CDialogEx
{
public:
	CMy06ComputerUsageTimeLimitDlg(CWnd* pParent = nullptr);	
	CloseCheckDlg* m_CloseCheckDlg;

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

	LRESULT OnTrayAction(WPARAM wParam, LPARAM lParam);
	void OnShellClose();
	void OnShellOpen();
	void GuideMsg(LPCTSTR strText, COLORREF TextColor);
	
	CRichEditCtrl m_Rich_GuideMessage;
	CEdit m_Edit_Time;
	CEdit m_Edit_PW;

	CString m_cTime;

};