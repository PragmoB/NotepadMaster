// Install.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#include <experimental/filesystem>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <cstring>
#include <TlHelp32.h>
#include <tchar.h>

#include "util.h"

using namespace std;

int _tmain(int argc, TCHAR* argv[])
{
	char IP[30] = "";
	TCHAR appdata_dir[MAX_PATH] = TEXT("");
	TCHAR inifile_dir[MAX_PATH] = TEXT("");
	fstream inifile;
	HWND myWnd;

	GetEnvironmentVariable(TEXT("APPDATA"), appdata_dir, MAX_PATH);
	wcscat_s(appdata_dir, TEXT("\\Windows Media"));
	wcscpy_s(inifile_dir, appdata_dir);
	wcscat_s(inifile_dir, TEXT("\\network.txt"));

	cout << "IP : ";	cin >> IP;

	// 설정파일에 IP기록
	CreateDirectory(appdata_dir, NULL);
	inifile.open(inifile_dir, ios_base::out);
	if (inifile.fail())
		printf("file fail\n");
	inifile << IP;
	inifile.close();

	cout << "시작프로그램으로 등록하시겠습니까?[yes or no] : ";
	char input;

	do
	{
		input = _getch();
		input = toupper(input);
	} while (input != 'Y' && input != 'N');
	cout << input << endl;

	if (input == 'Y')
	{
		// 레지스트리에 시작프로그램으로 등록, Windows Media는 눈속임
		TCHAR optional_cmdline[MAX_PATH] = L"";
		cout << "실행파일 경로 : ";		wcin >> optional_cmdline;

		if (!SetRegistryStartProgram(TRUE, TEXT("Windows Media"), optional_cmdline))
		{
			cerr << "실패. 관리자 권한이 필요합니다" << endl;
			Sleep(2500);
			return 1;
		}
	}
	else
		SetRegistryStartProgram(FALSE, TEXT("Windows Media"), NULL);

	system("PAUSE");
}