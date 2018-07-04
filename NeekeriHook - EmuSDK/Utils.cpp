#include "SDK.h"

/* implementation */
CUtils* g_pUtils = new CUtils();
FILE* g_flLog = NULL;

/* macros */
#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

void CUtils::Log(const char* fmt, ...)
{
	if (!fmt) return; 

	va_list va_alist;
	char logBuf[256] = { 0 };

	va_start(va_alist, fmt);
	_vsnprintf(logBuf + strlen(logBuf), sizeof(logBuf) - strlen(logBuf), fmt, va_alist);
	va_end(va_alist);

	if (logBuf[0] != '\0')
	{
		SYSTEMTIME time;
		GetLocalTime(&time);
		printf("[%02d:%02d:%02d]", time.wHour, time.wMinute, time.wSecond);
		printf(": %s\n", logBuf);
	}
	
	std::ofstream file;

	/* filthy hacking */
	file.open(std::string(CHEAT_NAME).append(".log"), std::ios::app);

	file << logBuf << std::endl;
	
	file.close();
}

/* actually cancer function */
HMODULE CUtils::GetHandleSafe(std::string handle)
{
	FUNCTION_GUARD;

	while(!GetModuleHandleA(handle.c_str())) Sleep(50);

	return GetModuleHandleA(handle.c_str());
}

std::uint8_t* CUtils::PatternScan(void* module, const char* signature)
{
	static auto pattern_to_byte = [](const char* pattern) {
		auto bytes = std::vector<int>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current) {
			if (*current == '?') {
				++current;
				if (*current == '?')
					++current;
				bytes.push_back(-1);
			}
			else {
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	auto dosHeader = (PIMAGE_DOS_HEADER)module;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)module + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = pattern_to_byte(signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(module);

	auto s = patternBytes.size();
	auto d = patternBytes.data();

	for (auto i = 0ul; i < sizeOfImage - s; ++i) {
		bool found = true;
		for (auto j = 0ul; j < s; ++j) {
			if (scanBytes[i + j] != d[j] && d[j] != -1) {
				found = false;
				break;
			}
		}
		if (found) {
			return &scanBytes[i];
		}
	}
	return nullptr;
}


DWORD CUtils::FindPatternIDA(std::string moduleName, std::string pattern) // learn_more
{
	FUNCTION_GUARD;

	const char* pat = pattern.c_str();
	DWORD firstMatch = 0;
	DWORD rangeStart = (DWORD)GetModuleHandleA(moduleName.c_str());
	MODULEINFO miModInfo;
	K32GetModuleInformation(GetCurrentProcess(), (HMODULE)rangeStart, &miModInfo, sizeof(MODULEINFO));
	DWORD rangeEnd = rangeStart + miModInfo.SizeOfImage;
	for (DWORD pCur = rangeStart; pCur < rangeEnd; pCur++)
	{
		if (!*pat)
			return firstMatch;

		if (*(PBYTE)pat == '\?' || *(BYTE*)pCur == getByte(pat))
		{
			if (!firstMatch)
				firstMatch = pCur;

			if (!pat[2])
				return firstMatch;

			if (*(PWORD)pat == '\?\?' || *(PBYTE)pat != '\?')
				pat += 3;

			else
				pat += 2; //one ?
		}
		else
		{
			pat = pattern.c_str();
			firstMatch = 0;
		}
	}
	
	Log(" -- The pattern '%s' in '%s' returned no valid result.", pattern, moduleName.c_str());
	return NULL;
}

bool CUtils::ByteCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}

DWORD CUtils::FindPatternByte(std::string moduleName, PBYTE Mask, char* szMask)
{
	FUNCTION_GUARD;

	DWORD Address = reinterpret_cast<DWORD>(GetHandleSafe(moduleName));
	MODULEINFO ModInfo; 
	GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (ByteCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return (DWORD)(Address + c);
		}
	}

	Log(" -- The byte pattern '%x' with mask '%s' in '%s' returned no valid result.", Mask, szMask, moduleName);
	return 0;
}

/* dont judge me for putting these here */
using InitKeyValuesFn = void(__thiscall*)(void* thisptr, const char* name);
using LoadFromBufferFn = void(__thiscall*)(void* thisptr, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc);

void CUtils::InitKeyValues(KeyValues* pKeyValues, const char* name) 
{
	InitKeyValuesFn InitKeyValuesEx = reinterpret_cast<InitKeyValuesFn>(FindPatternIDA("client.dll", "55 8B EC 51 33 C0 C7 45"));
	InitKeyValuesEx(pKeyValues, name);
}

void CUtils::LoadFromBuffer(KeyValues* pKeyValues, const char* resourceName, const char* pBuffer, void* pFileSystem, const char* pPathID, void* pfnEvaluateSymbolProc) 
{
	LoadFromBufferFn LoadFromBufferEx = reinterpret_cast<LoadFromBufferFn>(FindPatternIDA("client.dll", "55 8B EC 83 E4 F8 83 EC 34 53 8B 5D 0C 89 4C 24 04"));
	LoadFromBufferEx(pKeyValues, resourceName, pBuffer, pFileSystem, pPathID, pfnEvaluateSymbolProc);
}