#pragma once
#include <afxsock.h>

class CNotepadMasterServerDlg;

class CListenSocket :
	public CSocket
{
public:
	CNotepadMasterServerDlg* main_dlg;
public:
	CListenSocket(CNotepadMasterServerDlg*);
	virtual void OnAccept(int nErrorCode);
};

