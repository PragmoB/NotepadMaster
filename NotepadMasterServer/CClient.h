#pragma once

#include <vector>

#include "CClientSocket.h"
#include "CClientKeylog.h"

#include "definition.h"

const UINT NORMAL_FONT_SIZE = 25;

// CClient 대화 상자

class CClient : public CDialogEx
{
	DECLARE_DYNAMIC(CClient)

public:
	CClient(CWnd* pParent = nullptr);   // 표준 생성자입니다.
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
};
