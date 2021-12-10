#include "pch.h"
#include "CListenSocket.h"
#include "NotepadMasterServerDlg.h"

CListenSocket::CListenSocket(CNotepadMasterServerDlg* main_dlg)
{
	this->main_dlg = main_dlg;
}
void CListenSocket::OnAccept(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	main_dlg->ClientAccept();
	CSocket::OnAccept(nErrorCode);
}
