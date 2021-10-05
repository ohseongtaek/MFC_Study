#pragma once


// TimeDlg 대화 상자

class TimeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(TimeDlg)

public:
	TimeDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TimeDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TIME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	// seongtaek10.oh 
	void GuideMsg3(LPCTSTR strText, COLORREF TextColor);

	CStatic m_ShowTime;
	CStatic m_EndTime;
	CRichEditCtrl m_rcGuide3;
};
