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
	if (len < 0)
	{
		return;
	}
	switch (buff[0])
	{
	case HELLO :
		PDUHello* pdu = (PDUHello*)buff;
		CString format(pdu->internal_ip);
		main_dlg->SetInvisibleInfo(this, format);
	}
	CSocket::OnReceive(nErrorCode);
}
