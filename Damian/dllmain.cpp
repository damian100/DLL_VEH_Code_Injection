#pragma once
#pragma comment(lib, "dwmapi.lib")
#include <Windows.h>
#include <dwmapi.h>
#include <fstream>
#include <iostream>
#include "USEAddr.h"
#include "USEBP.h"
#include "Close.h"
#include <TlHelp32.h>
#include <vector>
#include <algorithm>
#include <string>
#include <urlmon.h>
#include <thread>
#include <string>
#include <direct.h>
#include <urlmon.h>
#include <ShObjIdl.h>
#include <iphlpapi.h>
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "iphlpapi.lib" )
#pragma comment(lib, "urlmon.lib")
#pragma comment (lib, "winhttp.lib")
#pragma comment(lib, "iphlpapi.lib" )
#define _CRT_SECURE_NO_DEPRECATE

using namespace std;
#define LODWORD(l)   ((DWORD)((DWORDLONG)(l)))
#define HIDWORD(l)   ((DWORD)(((DWORDLONG)(l) >> 32) & 0xFFFFFFFF))





LONG WINAPI ExceptionHandler(EXCEPTION_POINTERS* e)
{
	if (e->ExceptionRecord->ExceptionAddress == (void*)(ProGressive->ModuleAddress + BPAddr::WriteFile))
	{
		e->ContextRecord->Rsp += 0xA0;
		__try
		{
			ProGressive->WPM(e->ContextRecord->Rdx,0x61);

			std::cout << e->ContextRecord->Rdx << std::endl;

			e->ContextRecord->Rcx = e->ContextRecord->R12; // mov rcx, r12
			e->ContextRecord->Rip = ProGressive->ModuleAddress + BPAddr::WriteFile + 0x3; // 다음 코드
			return EXCEPTION_CONTINUE_EXECUTION;
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			e->ContextRecord->Rcx = e->ContextRecord->R12;
			e->ContextRecord->Rip = ProGressive->ModuleAddress + BPAddr::WriteFile + 0x3;
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void USEVEH()
{
	HANDLE hMainThread = BLACKFAST->GetMainThread();
	PVOID pHandler = AddVectoredExceptionHandler(1, ExceptionHandler);
	CONTEXT c{};
	c.ContextFlags = CONTEXT_DEBUG_REGISTERS;
	SuspendThread(hMainThread);
	
	c.Dr0 = ProGressive->ModuleAddress + BPAddr::WriteFile;
	//c.Dr1 = ProGressive->ModuleAddress + 
	//c.Dr2 = ProGressive->ModuleAddress + 
	c.Dr7 = (1 << 0) | (1 << 2) | (1 << 4) | (1 << 6);
	SetThreadContext(hMainThread, &c);
	ResumeThread(hMainThread);
}


DWORD WINAPI Main(LPVOID lpParam)
{
	AllocConsole();

	ProGressive->Initialize();
	std::cout << "complete!" << std::endl;
	USEVEH();
	while (true)
	{

	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		
		DisableThreadLibraryCalls(hModule);
		CreateThread(NULL, 0, Main, lpReserved, 0, NULL);
	
	}
	return TRUE;
}
