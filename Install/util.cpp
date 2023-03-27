#include <windows.h>
#include <TlHelp32.h>
#include <string>

BOOL SetRegistryStartProgram(BOOL bAutoExec, LPCWSTR lpValueName, LPCWSTR lpExeFileName)
{
	HKEY hKey;
	LONG lRes;
	if (bAutoExec)
	{
		if (lpValueName == NULL || lpExeFileName == NULL)
			return FALSE;
		if (RegOpenKeyEx(
			HKEY_LOCAL_MACHINE,
			L"SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run\\",
			0L, KEY_WRITE,
			&hKey
		) != ERROR_SUCCESS)
			return FALSE;

		lRes = RegSetValueEx(hKey,
			lpValueName,
			0,
			REG_SZ,
			(BYTE*)lpExeFileName,
			lstrlen(lpExeFileName) * 2);

		RegCloseKey(hKey);

		if (lRes != ERROR_SUCCESS)
			return FALSE;
	}
	else
	{
		if (RegOpenKeyEx(
			HKEY_LOCAL_MACHINE,
			L"SOFTWARE\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Run\\",
			0, KEY_ALL_ACCESS,
			&hKey) != ERROR_SUCCESS)
			return FALSE;

		lRes = RegDeleteValue(hKey, lpValueName);

		RegCloseKey(hKey);

		if (lRes != ERROR_SUCCESS)
			return FALSE;
	}

	return TRUE;
}