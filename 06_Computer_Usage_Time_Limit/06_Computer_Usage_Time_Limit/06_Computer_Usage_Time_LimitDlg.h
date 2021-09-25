#pragma once

#define WM_TRAY_NOTIFYCATION WM_APP + 10

class CMy06ComputerUsageTimeLimitDlg : public CDialogEx
{
public:
	CMy06ComputerUsageTimeLimitDlg(CWnd* pParent = nullptr);	

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
	LRESULT OnTrayAction(WPARAM wParam, LPARAM lParam);
	void OnShellClose();
	void OnShellOpen();
	void GuideMsg(LPCTSTR strText, COLORREF TextColor);
	afx_msg void OnBnClickedOk();
	CRichEditCtrl m_Rich_GuideMessage;
};
