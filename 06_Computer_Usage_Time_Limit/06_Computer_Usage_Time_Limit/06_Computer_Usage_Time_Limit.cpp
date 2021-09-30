#include "pch.h"
#include "framework.h"
#include "06_Computer_Usage_Time_Limit.h"
#include "06_Computer_Usage_Time_LimitDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMy06ComputerUsageTimeLimitApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CMy06ComputerUsageTimeLimitApp::CMy06ComputerUsageTimeLimitApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}

// 유일한 CMy06ComputerUsageTimeLimitApp 개체입니다.

CMy06ComputerUsageTimeLimitApp theApp;


BOOL CMy06ComputerUsageTimeLimitApp::InitInstance()
{
// TODO: richedit2 라이브러리를 초기화하려면 AfxInitRichEdit2()을(를) 호출합니다.\n"	CWinApp::InitInstance();


	// 대화 상자에 셸 트리 뷰 또는
	// 셸 목록 뷰 컨트롤이 포함되어 있는 경우 셸 관리자를 만듭니다.
	CShellManager *pShellManager = new CShellManager;

	// MFC 컨트롤의 테마를 사용하기 위해 "Windows 원형" 비주얼 관리자 활성화
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 애플리케이션 마법사에서 생성된 애플리케이션"));

	AfxInitRichEdit();

	CMy06ComputerUsageTimeLimitDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

	// 위에서 만든 셸 관리자를 삭제합니다.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	return FALSE;
}

std::string CMy06ComputerUsageTimeLimitApp::strToMD5(CString pw)
{
	CT2CA convertedString(pw);
	std::string strMD5 = std::string(convertedString);

	md5_state_t state;
	md5_byte_t digest[16];
	char hex_output[16 * 2 + 1];

	md5_init(&state);
	md5_append(&state, (const md5_byte_t*)strMD5.c_str(), strMD5.length());
	md5_finish(&state, digest);

	for (int i = 0; i < 16; i++)
	{
		sprintf(hex_output + i * 2, "%02x", digest[i]);
	}

	return hex_output;
}