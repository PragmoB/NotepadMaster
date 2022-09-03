#pragma once

#include <windows.h>
#include <TlHelp32.h>

DWORD FindProcessID(LPCTSTR szProcessName);
void FindProcessName(DWORD szProcessID, WCHAR* dest);
void ChangeFont(HWND h_edit, int font_size, const WCHAR* font_name);
void replaceAll(std::string& str, const char* target, const char* result);
BOOL SetRegistryStartProgram(BOOL bAutoExec, LPCWSTR lpValueName, LPCWSTR lpExeFileName);