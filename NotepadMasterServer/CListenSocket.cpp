#include "pch.h"
#include "CListenSocket.h"
#include "NotepadMasterServerDlg.h"

CListenSocket::CListenSocket(CNotepadMasterServerDlg* main_dlg)
{
	this->main_dlg = main_dlg;
}
void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	main_dlg->ClientAccept();
	CSocket::OnAccept(nErrorCode);
}
