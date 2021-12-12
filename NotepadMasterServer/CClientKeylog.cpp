// CClientKeylog.cpp: 구현 파일
//

#include "pch.h"
#include "NotepadMasterServer.h"
#include "CClientKeylog.h"
#include "afxdialogex.h"


// CClientKeylog 대화 상자

IMPLEMENT_DYNAMIC(CClientKeylog, CDialogEx)

CClientKeylog::CClientKeylog(CString process_name, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CLIENT_KEYLOG, pParent)
	, m_client_keylog(_T(""))
{
	this->process_name = process_name;
}

CClientKeylog::~CClientKeylog()
{
}

void CClientKeylog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_CLIENT_KEYLOG, m_client_keylog);
}


BEGIN_MESSAGE_MAP(CClientKeylog, CDialogEx)
END_MESSAGE_MAP()


// CClientKeylog 메시지 처리기


CString CClientKeylog::GetProcessName()
{
	// TODO: 여기에 구현 코드 추가.
	return process_name;
}
