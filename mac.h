#pragma once

#include <stdio.h>
#include <Windows.h>
#include <Iphlpapi.h>
#include <string>
#include <Assert.h>

#include "mac.h"
#include "Vaults.h"
#pragma comment(lib, "iphlpapi.lib")

struct MacInfo
{
	char* cAddr;
	int addr[6];
	int addon;
};

class SecurityMacSystem
{
public:
	bool is_administrator = false;
	bool is_dev = false;
public:
	bool is_valid()
	{
		for (auto user : vaults.users)
		{
			bool correct[6] = { false, false, false, false, false, false };
			for (int i = 0; i < 6; i++)
			{
				if (get_mac().addr[i] == user.addr[i])
				{
					correct[i] = true;
				}
			}
			if (get_mac().addon == user.mac_int)
				return correct[0] && correct[1] && correct[2] && correct[3] && correct[4] && correct[5];
		}
		return false;
	}

	std::string get_name()
	{
		for (auto user : vaults.users)
		{
			bool correct[6] = { false, false, false, false, false, false };
			for (int i = 0; i < 6; i++)
			{
				if (get_mac().addr[i] == user.addr[i]) correct[i] = true;
			}
			if (get_mac().addon == user.mac_int)
				return user.name;
		}
	}

	MacInfo get_mac() {
		PIP_ADAPTER_INFO AdapterInfo;
		DWORD dwBufLen = sizeof(AdapterInfo);
		char *mac_addr = (char*)malloc(17);

		AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW)
			AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);

		if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR)
		{
			PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
			do {
				sprintf(mac_addr, "%02X:%02X:%02X:%02X:%02X:%02X",
					pAdapterInfo->Address[0], pAdapterInfo->Address[1],
					pAdapterInfo->Address[2], pAdapterInfo->Address[3],
					pAdapterInfo->Address[4], pAdapterInfo->Address[5]);

				MacInfo info;
				info.cAddr = mac_addr;
				info.addr[0] = pAdapterInfo->Address[0];
				info.addr[1] = pAdapterInfo->Address[1];
				info.addr[2] = pAdapterInfo->Address[2];
				info.addr[3] = pAdapterInfo->Address[3];
				info.addr[4] = pAdapterInfo->Address[4];
				info.addr[5] = pAdapterInfo->Address[5];
				info.addon = pAdapterInfo->Address[0] + pAdapterInfo->Address[1]
					+ pAdapterInfo->Address[2] + pAdapterInfo->Address[3]
					+ pAdapterInfo->Address[4] + pAdapterInfo->Address[5];

				return info;

				pAdapterInfo = pAdapterInfo->Next;
			} while (pAdapterInfo);
		}
		free(AdapterInfo);
	}
};
extern SecurityMacSystem macsys;