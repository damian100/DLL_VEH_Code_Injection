#pragma once
#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>

class Module
{
public:
	HANDLE hProcess;
	DWORD64 ModuleAddress;

	void Initialize()
	{
		hProcess = GetCurrentProcess();
		ModuleAddress = (DWORD64)GetModuleHandleA("Notepad.exe");
	}

	template <typename RPMType>
	RPMType RPM(DWORD64 Address)
	{
		RPMType Buffer;
		ReadProcessMemory(hProcess, LPVOID(Address), &Buffer, sizeof(Buffer), NULL);
		return Buffer;
	}

	template <typename WPMType>
	BOOL WPM(DWORD64 Address, WPMType value)
	{
		return WriteProcessMemory(hProcess, LPVOID(Address), &value, sizeof(value), NULL);
	}
};

extern Module* ProGressive = new Module();