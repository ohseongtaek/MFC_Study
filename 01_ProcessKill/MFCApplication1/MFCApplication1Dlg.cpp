
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

// 프로세스를 담을 Map 자료구조 
std::map<DWORD, TCHAR*> ProcessList;
std::map<DWORD, TCHAR*>::iterator ProcessList_iter;


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
	DDX_Control(pDX, IDC_LIST1, m_ProcessListBox);
	DDX_Control(pDX, IDC_BUTTON1, m_SelectKillButton);
	DDX_Control(pDX, IDC_EDIT2, m_InputPID);
	DDX_Control(pDX, IDC_BUTTON2, m_PIDKill);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CMFCApplication1Dlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CMFCApplication1Dlg::OnBnClickedOk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CMFCApplication1Dlg::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT2, &CMFCApplication1Dlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON2, &CMFCApplication1Dlg::OnBnClickedButton2)
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

	// List Control Box Init 	
	m_ProcessListBox.DeleteAllItems();
	m_ProcessListBox.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_ProcessListBox.InsertColumn(0, _T("Number"), LVCFMT_LEFT, 80, -1);
	m_ProcessListBox.InsertColumn(1, _T("Process Name"), LVCFMT_LEFT, 240, -1);
	m_ProcessListBox.InsertColumn(2, _T("Process ID"), LVCFMT_LEFT, 243, -1);

	// List Control Box List Up Function 
	ProcessListUP(ProcessList);
	

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
	// 취소 버튼을 누르면 나오는 메시지 
	CDialogEx::OnCancel();
	MessageBox(_T("취소버튼을 누르셨습니다."));

}


void CMFCApplication1Dlg::OnBnClickedOk()
{
	// 확인 버튼을 누르면 나오는 메시지 
	CDialogEx::OnOK();
	MessageBox(_T("확인버튼을 누르셨습니다."));
}

void CMFCApplication1Dlg::OnLvnItemchangedList1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// Seongtaek => This code Do not AnyThing
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	*pResult = 0;
}


void CMFCApplication1Dlg::OnBnClickedButton1()
{
	// PID 작업종료 Function 
	HANDLE  hProcess		  = NULL;
	CString ProcessName		  = NULL;
	CString ProcessID		  = NULL;
	DWORD   dwProcessID		  = 0;
	DWORD   dwExitCode		  = 0;
	TCHAR  tszText[MAX_PATH] = { 0, };
	
	// 목록에서 선택된 값 찾기 
	for (int i = 0; i < m_ProcessListBox.GetItemCount(); i++)
	{
		if (LVIS_SELECTED == m_ProcessListBox.GetItemState(i, LVIS_SELECTED))
		{
			ProcessName = m_ProcessListBox.GetItemText(i, 1);
			ProcessID   = m_ProcessListBox.GetItemText(i, 2);
			dwProcessID = atol((char*)(LPCTSTR)ProcessID);
			break;
		}
	}

	// 프로세스 죽이기 
	if (ProcessList.find(dwProcessID) != ProcessList.end())
	{
		if (NULL == (hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID)))
		{
			MessageBox("Process is Null");
			return;
		}
		if (0 == GetExitCodeProcess(hProcess, &dwExitCode))
		{
			MessageBox("Fail Get Exit Code");
			return;
		}
		if (0 == (TerminateProcess(hProcess, dwExitCode)))
		{
			MessageBox("Fail Terminate Process");
			return;
		}
		
		// 종료 성공시 보이는 메시지 
		sprintf_s(tszText,sizeof(tszText), "%s가 종료되었습니다.", ProcessName);
		MessageBox(tszText,"종료",MB_OK);

		// Map Data 다시 읽어 들이기 
		ProcessList.clear();
		ProcessListUP(ProcessList);

		// 화면 다시 그리기 
		Invalidate(FALSE);
	}
	else
	{
		MessageBox(_T("Map Data 와 동기화 오류가 있습니다."));
	}
}


void CMFCApplication1Dlg::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.
}


void CMFCApplication1Dlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Input PID Kill Function 

	HANDLE  hProcess = NULL;
	CStringA ProcessName = NULL;
	CString ProcessID = NULL;
	DWORD   dwProcessID = 0;
	DWORD   dwExitCode = 0;
	TCHAR   tszText[MAX_PATH] = { 0, };

	m_InputPID.GetWindowText(ProcessID);
	dwProcessID = atol((char*)(LPCTSTR)ProcessID);

	// 프로세스 죽이기
	for (ProcessList_iter = ProcessList.begin(); ProcessList_iter != ProcessList.end(); ProcessList_iter++)
	{
		if (ProcessList_iter->first == dwProcessID)
		{
			ProcessName = CStringA(ProcessList_iter->second);
			
			if (NULL == (hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID)))
			{
				MessageBox("Process is Null");
				return;
			}
			if (0 == GetExitCodeProcess(hProcess, &dwExitCode))
			{
				MessageBox("Fail Get Exit Code");
				return;
			}
			if (0 == (TerminateProcess(hProcess, dwExitCode)))
			{
				MessageBox("Fail Terminate Process");
				return;
			}

			// 종료 성공시 보이는 메시지 
			sprintf_s(tszText, sizeof(tszText), "%s가 종료되었습니다.", ProcessName);
			MessageBox(tszText, "종료", MB_OK);
			break;
		}
	}

	// Map Data 다시 읽어 들이기 
	ProcessList.clear();
	ProcessListUP(ProcessList);

	// 화면 다시 그리기 
	Invalidate(FALSE);
}

void CMFCApplication1Dlg::ProcessListUP(std::map<DWORD, TCHAR*> &temp)
{
	PROCESSENTRY32 ProcessEntry32 = { 0, };
	ProcessEntry32.dwSize		  = sizeof(PROCESSENTRY32);
	BOOL bProcessFound			  = FALSE;
	CString CListFormat			  = NULL;
	int nidx					  = 0;

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		exit(EXIT_FAILURE);
	}

	bProcessFound = Process32First(hSnapshot, &ProcessEntry32);
	while (bProcessFound)
	{
		temp.insert(std::make_pair(ProcessEntry32.th32ProcessID, ProcessEntry32.szExeFile));
		
		// Number Column
		CListFormat.Format(_T("%d"), nidx);
		m_ProcessListBox.InsertItem(nidx, CListFormat);
		
		// Process Name Column
		m_ProcessListBox.SetItem(nidx, INDEX_PROCESSNAME_COLUMN, LVIF_TEXT, ProcessEntry32.szExeFile, 0, 0, 0, NULL);

		// Process ID Column
		CListFormat.Format(_T("%d"), ProcessEntry32.th32ProcessID);
		m_ProcessListBox.SetItem(nidx, INDEX_PROCESSID_COLUMN, LVIF_TEXT, CListFormat, 0, 0, 0, NULL);
		
		// Index ++ 
		nidx++;
		bProcessFound = Process32Next(hSnapshot, &ProcessEntry32);
	}
	CloseHandle(hSnapshot);	
}