
// MFCApplication1Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <string>
#include <map>
#include <vector>



#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMFCApplication1Dlg 대화 상자



CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, cbEnter);
	DDX_Control(pDX, IDCANCEL, cbCancel);
	DDX_Control(pDX, IDC_LIST2, clProcessList);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_LIST2, &CMFCApplication1Dlg::OnLbnSelchangeList2)
END_MESSAGE_MAP()


// CMFCApplication1Dlg 메시지 처리기

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	std::map<DWORD, TCHAR*> ProcessList;
	// Process List Up Function
	ProcessListUP(ProcessList);
	char temp[MAX_PATH] = { 0, };
	sprintf_s(temp, sizeof(temp), "%d", ProcessList.size());
	clProcessList.AddString(temp);

	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
// 아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
// 프레임워크에서 이 작업을 자동으로 수행합니다.
void CMFCApplication1Dlg::OnPaint()
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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication1Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication1Dlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();

	// 확인 버튼을 누르면 나오는 메시지 
	MessageBox(_T("취소버튼을 누르셨습니다."));

}


void CMFCApplication1Dlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
	MessageBox(_T("확인버튼을 누르셨습니다."));
}


void CMFCApplication1Dlg::OnLbnSelchangeList2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

}

void CMFCApplication1Dlg::ProcessListUP(std::map<DWORD, TCHAR*> &temp)
{
	PROCESSENTRY32 ProcessEntry32 = { 0, };
	ProcessEntry32.dwSize		  = sizeof(PROCESSENTRY32);
	BOOL bProcessFound			  = FALSE;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		exit(EXIT_FAILURE);
	}

	bProcessFound = Process32First(hSnapshot, &ProcessEntry32);
	while (bProcessFound)
	{
		temp.insert(std::make_pair(ProcessEntry32.th32ProcessID, ProcessEntry32.szExeFile));
		//clProcessList.AddString(ProcessEntry32.szExeFile);
		//clProcessList.AddString(LPTSTR(ProcessEntry32.cntThreads));
		bProcessFound = Process32Next(hSnapshot, &ProcessEntry32);
	}
	CloseHandle(hSnapshot);
	// End
	//clProcessList.AddString(LPCTSTR(temp.size()));
	
}
// Read 

