#include "Mem.h"

//--TODO: Needs to return a vector of all found values instead of 1 return.
#if defined(_WIN32)
void* SignatureScan(char* signature, unsigned int size)
{
	MEMORY_BASIC_INFORMATION pageInfo;
	void* pageBeginPtr = nullptr;

	while (VirtualQuery(pageBeginPtr, &pageInfo, sizeof(MEMORY_BASIC_INFORMATION)))
	{
		DWORD oldprotect;
		VirtualProtect(pageBeginPtr, pageInfo.RegionSize, PAGE_EXECUTE_READWRITE, &oldprotect);

#if defined(_SLIB_SIGNATURE_SCAN_DEBUG_PAGE_INFO_)
		DEBUG_OUTF("START: %p - ", pageBeginPtr);
		DEBUG_OUTF("END: %p : (PROTECTION)(", (char*)pageBeginPtr + pageInfo.RegionSize);
		DEBUG_OUTF("%p)\n", pageInfo.Protect);
#endif
		if (pageInfo.State == MEM_COMMIT && (pageInfo.Protect == PAGE_READWRITE || pageInfo.Protect == PAGE_EXECUTE_READ || pageInfo.Protect == PAGE_EXECUTE_READWRITE))
		{
			for (unsigned long long i = 0; i < pageInfo.RegionSize - size; i++)
			{
				char* store = ((char*)pageBeginPtr + i);
				if (store == signature)
				{
					break;
				}
				int totalMatched = 0;
				for (int j = 0; j < sizeof(signature); j++)
				{
					if (store[j] == signature[j])
					{
						totalMatched++;
					}
				}
				if (totalMatched == size)
				{
					return store;
				}
			}
		}
		pageBeginPtr = (char*)pageInfo.BaseAddress + pageInfo.RegionSize;
	}
	return nullptr;
}
DWORD GetModuleBaseAddr(DWORD processID, const char* moduleName)
{
	DWORD baseAddress = NULL;

	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processID);

	if (snapshot == INVALID_HANDLE_VALUE)
	{
		DEBUG_OUT("[SLIB GetModuleBaseAddr] Unable to create process snapshot.");
		return NULL;
	}
	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(snapshot, &moduleEntry))
	{
		do
		{
			if (!strcmp(moduleName, moduleEntry.szModule))
			{
				return (DWORD)moduleEntry.modBaseAddr;
			}
		} while (Module32Next(snapshot, &moduleEntry));
	}
	return NULL;
};

#endif


