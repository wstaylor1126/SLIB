#include "Mem.h"

//--Throwaway funcs
int ByteArrCmp_Mask(const char* cmp1, const char* cmp2, unsigned int size, const char* mask)
{
	for (int i = 0; i < size; i++)
	{
		if (mask[i] == '%')
		{
			continue;
		}
		if (cmp1[i] != cmp2[i])
		{
			return 0;
		}
	}
	return 1;
};

//--TODO: Needs to return a vector of all found values instead of 1 return.
#if defined(_WIN32)
void* SignatureScan(const char* signature, unsigned int size, const char* mask)
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
				if (ByteArrCmp_Mask(signature, store, size, mask))
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


