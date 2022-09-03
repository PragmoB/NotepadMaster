#pragma once

#include <vector>

#include "definition.h"

// CClientKeylog 대화 상자

class CClientKeylog : public CDialogEx
{
	DECLARE_DYNAMIC(CClientKeylog)

public:
	CClientKeylog(CString process_name, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CClientKeylog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CLIENT_KEYLOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CString process_name;
public:
//	CString m_client_keylog;
public:
	CString GetProcessName();
	CEdit m_edit_client_keylog;
	CString m_client_keylog;
	void PrintKeylog();
	int m_keylog_lang;
	CString m_client_keylog_ko;
	void ScanKeylog(PDUKeylog* pdu);
	afx_msg void OnClickedRadioKeylogEn(UINT uiID);
	afx_msg void OnVscrollEditClientKeylog();
	virtual BOOL OnInitDialog();
	void SaveKeylog(CString IP, int index);
};
