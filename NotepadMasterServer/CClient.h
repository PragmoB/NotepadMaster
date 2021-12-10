#pragma once

#include "CClientSocket.h"

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
public:
	int m_msg_kind;
	CString m_command;
	CTabCtrl m_tab_client_keylog;
	CClientSocket* socket;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSend();
};
