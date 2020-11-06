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
        cout << "���� ȣ��Ʈ : " << inet_ntoa(*(struct in_addr*)gethostbyname(hostname)->h_addr_list[0]) << endl << endl;

    cout << "���� ��ȣ ����Ʈ" << endl;
    for(int i = 0;i < Clients.size();i++)
    {
        if(i % 5 == 0) // ���Ϲ�ȣ 5������ �ڸ� ���.
        {
            cout << endl;
        }
        cout << Clients[i] << " | ";
    }
    cout << endl << endl;
    cout << "Ÿ�� ���Ϲ�ȣ : ";
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
        cerr << "�ʱ�ȭ ����. �����ڵ�: " << WSAGetLastError();
        return 1;
    }
    cout << "�ʱ�ȭ ����" << endl;

    if((binding_socket = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        cerr << "���� ���� ����. �����ڵ�: " << WSAGetLastError();
        WSACleanup();
        getch();
        return 1;
    }
    cout << "���� ���� ����" << endl;

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(44401);

    int nSockOpt = 1;
    setsockopt(binding_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&nSockOpt, sizeof(nSockOpt));
    if (-1 == bind (binding_socket, (struct sockaddr *) &server, sizeof (server)))
    {
        cerr << " bind()���� ����";
        WSACleanup();
        closesocket(binding_socket);
        getch();
        exit(1);
    }
    cout << "bind()���� ����" << endl;

    if (-1 == listen (binding_socket, 5))
    {
        cerr << "������ ��� ���� ����.";
        WSACleanup();
        closesocket(binding_socket);
        getch();
        exit(1);
    }
    cout << "������ ��� ���� ����" << endl;
    cout << endl;

    hThread = (HANDLE)_beginthreadex(NULL,0, active , (void*)&Clients, 0, (unsigned*)&dwThreadID);
    if(hThread == 0)
        cerr << "beginhandlethreadex error";

    while(1)
    {
        if((temp = accept(binding_socket, (struct sockaddr *) &client_addr, &client_addr_size)) == -1)
        {
            cerr << "Ŭ���̾�Ʈ ���� ���� ����.";
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
        cout << "�޽��� �Է�" << endl;

        while(TRUE)
        {
            cin.getline(buff_send, BUFF_SIZE);

            if(!strcmp(buff_send, "-exit")) // �ٸ� ���� ��ȣ�� �����ϱ� ���� Ż��.
                break;

            send(socks, buff_send, BUFF_SIZE, 0);
            memset(buff_send, 0, BUFF_SIZE);
        }
    }
}
