#pragma once
#include <afxsock.h>

class CNotepadMasterServerDlg;

class CClientSocket :
	public CSocket
{
public:
	CNotepadMasterServerDlg* main_dlg;
public:
	CClientSocket(CNotepadMasterServerDlg* main_dlg);
	virtual void OnReceive(int nErrorCode);
};

