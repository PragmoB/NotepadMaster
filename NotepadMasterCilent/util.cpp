#include <windows.h>
#include <TlHelp32.h>
#include <string>

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
void FindProcessName(DWORD szProcessID, WCHAR* dest)
{
	HANDLE hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32 pe;

	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);

	Process32First(hSnapShot, &pe);
	do
	{
		if (pe.th32ProcessID == szProcessID)
		{
			wcscpy_s(dest, 70, pe.szExeFile);
			break;
		}
	} while (Process32Next(hSnapShot, &pe));

	CloseHandle(hSnapShot);
}
void ChangeFont(HWND h_edit, int font_size, const WCHAR* font_name)
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
	PostMessage(h_edit, WM_SETFONT, (WPARAM)Font, 1); // note: WM_SETFONT�޼��� ������ �Ѵٴ°� ���ۿ��� �ȳ��� �־ ���� �������ϸ鼭 �˾Ƴ� �Ф�

	ReleaseDC(h_edit, hdc);
}

void replaceAll(std::string& str, const char* target, const char* result)
{
	std::string::size_type idx = str.find(target);
	while (idx != std::string::npos)
	{
		str.replace(idx, strlen(target), result);
		idx = str.find(target);
	}

}