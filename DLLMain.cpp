#include "DLLMain.h"
#include "Offsets.h"
#include "Interfaces.h"
#include "Hooks.h"
#include "DrawManager.h"
#include "Menu.h"
#include "Dumping.h"
#include "mac.h"
#include "Backtracking.h"
#include "Vaults.h"

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <TlHelp32.h>
#include <string>
#include <fstream>

extern HINSTANCE hAppInstance;

HINSTANCE HThisModule;
bool DoUnload;

void setFontSize(int FontSize)
{
	CONSOLE_FONT_INFOEX info = { 0 };
	info.cbSize = sizeof(info);
	info.dwFontSize.Y = FontSize;
	info.FontWeight = FW_NORMAL;
	wcscpy(info.FaceName, L"Tahoma");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), NULL, &info);
}

bool is_file_exist(const char *fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}

void create_folder(const char * path) {
	if (!CreateDirectory(path, NULL)) {
		return;
	}
}

void initialize_configs_file() {

	create_folder("yeti");
	create_folder("yeti\\cfg");

	if (!is_file_exist("yeti\\cfg\\yeti_configs.txt")) {
		std::ofstream("yeti\\cfg\\yeti_configs.txt");
	}
}

int InitialThread()
{

	initialize_configs_file();

	Offsets::Initialise();
	interfaces.initialise();
	NetVar.RetrieveClasses();
	NetvarManager::Instance()->CreateDatabase();
	Menu::SetupMenu();
	skinchanger.set_skins();
	Hooks::Initialise();

	while (DoUnload == false)
		Sleep(1000);

	Hooks::UndoHooks();
	Sleep(2000);
	FreeLibraryAndExitThread(HThisModule, 0);

	return 0;
}

const wchar_t *getwc(const char *c) {
	const size_t cSize = strlen(c) + 1;
	std::wstring wc(cSize, L'#');
	mbstowcs(&wc[0], c, cSize);
	return wc.data();
}

bool process_exists(const wchar_t* name, uint32_t& pid)
{
	auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (snapshot == INVALID_HANDLE_VALUE)
		return false;

	auto entry = PROCESSENTRY32{ sizeof(PROCESSENTRY32) };

	if (Process32First(snapshot, &entry)) {
		do {
			if (!wcscmp(getwc(entry.szExeFile), name)) {
				pid = entry.th32ProcessID;
				CloseHandle(snapshot);
				return true;
			}
		} while (Process32Next(snapshot, &entry));
	}
	CloseHandle(snapshot);
	return false;
}

uint32_t get_proc_id_by_name(char* process_name)
{
	PROCESSENTRY32   pe32;
	HANDLE         hSnapshot = NULL;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(hSnapshot, &pe32))
	{
		do {
			if (strcmp(pe32.szExeFile, process_name) == 0)
				break;
		} while (Process32Next(hSnapshot, &pe32));
	}

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);

	return pe32.th32ProcessID;
}


BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);


			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)InitialThread, NULL, NULL, NULL);

		return TRUE;
	}
	return TRUE;
}