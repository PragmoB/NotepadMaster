#pragma once

#include <vector>

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
	CString m_client_keylog;
public:
	CString GetProcessName();
};
