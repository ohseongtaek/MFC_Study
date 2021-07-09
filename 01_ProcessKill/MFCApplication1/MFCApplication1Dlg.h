
// MFCApplication1Dlg.h: 헤더 파일
//
#include <map>
#include <string>

#define INDEX_PROCESSNAME_COLUMN 1
#define INDEX_PROCESSID_COLUMN 2
#pragma once


// CMFCApplication1Dlg 대화 상자
class CMFCApplication1Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication1Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION1_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton cbEnter;
	CButton cbCancel;
	CListCtrl m_ProcessListBox; // 프로세스 리스트를 표시하는 리스트 컨트롤 박스
	CButton m_SelectKillButton; // Select 이후 프로세스 종료하는 버튼 
	CEdit m_InputPID;			// 종료를 원하는 Process ID 을 입력하는 edit 박스 
	CButton m_PIDKill;			// m_InputPID 변수에 입력된 PID 번호를 종료하는 버튼 

	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton2();

	//seongtaek function
	void ProcessListUP(std::map<DWORD, TCHAR*> &temp);	
};