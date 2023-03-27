#pragma once

#include <windows.h>
#include <TlHelp32.h>

BOOL SetRegistryStartProgram(BOOL bAutoExec, LPCWSTR lpValueName, LPCWSTR lpExeFileName);