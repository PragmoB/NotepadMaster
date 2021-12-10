// CClient.cpp: 구현 파일
//

#include "pch.h"
#include "NotepadMasterServer.h"
#include "CClient.h"
#include "afxdialogex.h"

#include "definition.h"

// CClient 대화 상자

IMPLEMENT_DYNAMIC(CClient, CDialogEx)

CClient::CClient(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_CLIENT, pParent)
	, m_msg_kind(0)
	, m_command(_T(""))
{

}

CClient::~CClient()
{
}

void CClient::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_MSG_KIND1, m_msg_kind);
	DDX_Text(pDX, IDC_EDIT_COMMAND, m_command);
	DDX_Control(pDX, IDC_TAB_CLIENT_KEYLOG, m_tab_client_keylog);
}


BEGIN_MESSAGE_MAP(CClient, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClient::OnBnClickedButtonSend)
END_MESSAGE_MAP()


// CClient 메시지 처리기


void CClient::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	DestroyWindow();
	CDialogEx::OnClose();
}


void CClient::OnBnClickedButtonSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_command.Replace(L"\r\n", L"\n");

	PDUSend pdu;
	wcscpy_s(pdu.message, m_command.GetBuffer());
	switch (m_msg_kind)
	{
	case 0:
		pdu.protocol_type = MESSAGE;
		break;

	case 1 :
		pdu.protocol_type = COMMAND;
		break;
	}
	socket->Send(&pdu, 1 + m_command.GetLength() * 2 + 1); // protocol_type(1byte) + command(unicode len * 2)
}
