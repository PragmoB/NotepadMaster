#include <iostream>
#include <windows.h>
#include <conio.h>
#include <vector>

#define BUFF_SIZE 2000

using namespace std;

vector<SOCKET> Clients;

unsigned int WINAPI active(void* arg);
void update()
{
    char hostname[50] = "";
    system("cls");

    if(gethostname(hostname, 50) == 0)
        cout << "현재 호스트 : " << inet_ntoa(*(struct in_addr*)gethostbyname(hostname)->h_addr_list[0]) << endl << endl;

    cout << "소켓 번호 리스트" << endl;
    for(int i = 0;i < Clients.size();i++)
    {
        if(i % 5 == 0) // 소켓번호 5개마다 자리 띄움.
        {
            cout << endl;
        }
        cout << Clients[i] << " | ";
    }
    cout << endl << endl;
    cout << "타겟 소켓번호 : ";
}

int main()
{

    WSADATA wsa;
    SOCKET binding_socket, temp;
    struct sockaddr_in server, client_addr;
    int client_addr_size;

    HANDLE hThread = NULL;
    DWORD dwThreadID = NULL;

    system("title Notepad Master");
    system("color f0");
    system("mode con cols=35 lines=20");

    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        cerr << "초기화 실패. 에러코드: " << WSAGetLastError();
        return 1;
    }
    cout << "초기화 성공" << endl;

    if((binding_socket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cerr << "소켓 생성 실패. 에러코드: " << WSAGetLastError();
        WSACleanup();
        getch();
        return 1;
    }
    cout << "소켓 생성 성공" << endl;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(44401);

    int nSockOpt = 1;
    setsockopt(binding_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nSockOpt, sizeof(nSockOpt));
    if (-1 == bind (binding_socket, (struct sockaddr *) &server, sizeof (server)))
    {
        cerr << " bind()실행 에러";
        WSACleanup();
        closesocket(binding_socket);
        getch();
        exit(1);
    }
    cout << "bind()실행 성공" << endl;

    if (-1 == listen (binding_socket, 5))
    {
        cerr << "대기상태 모드 설정 실패.";
        WSACleanup();
        closesocket(binding_socket);
        getch();
        exit(1);
    }
    cout << "대기상태 모드 설정 성공" << endl;
    cout << endl;

    hThread = (HANDLE)_beginthreadex(NULL,0, active , (void*)&Clients, 0, (unsigned*)&dwThreadID);
    if(hThread == 0)
        cerr << "beginhandlethreadex error";

    while(1)
    {
        if((temp = accept(binding_socket, (struct sockaddr *) &client_addr, &client_addr_size)) == -1)
        {
            cerr << "클라이언트 연결 수락 실패.";
        }
        Clients.push_back(temp);
        update();
    }
    return 0;
}

unsigned int WINAPI active(void* arg)
{
    char buff_send[BUFF_SIZE] = "";
    SOCKET socks;

    while(TRUE)
    {
        update();
        cin >> socks;
        while(!cin)
        {
            cin.clear();
            cin.ignore(INT_MAX, '\n');
            update();
            cin >> socks;
        }
        cin.getline(buff_send, BUFF_SIZE);
        cout << "메시지 입력" << endl;

        while(TRUE)
        {
            cin.getline(buff_send, BUFF_SIZE);

            if(!strcmp(buff_send, "-exit")) // 다른 소켓 번호를 선택하기 위해 탈출.
                break;

            send(socks, buff_send, BUFF_SIZE, 0);
            memset(buff_send, 0, BUFF_SIZE);
        }
    }
}
