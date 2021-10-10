#pragma once


// CloseCheckDlg 대화 상자

class CloseCheckDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CloseCheckDlg)

public:
	CloseCheckDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CloseCheckDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLOSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	void GuideMsg2(LPCTSTR strText, COLORREF TextColor);
	CRichEditCtrl m_reCloseMsg;
	CEdit m_edClosePW;

	afx_msg void PostNcDestroy();
	afx_msg void OnBnClickedButtonClose();
};
