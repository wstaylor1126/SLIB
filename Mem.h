#pragma once
#include "sLib.h"

#if defined(_WIN32)
void* SignatureScan(const char* signature, unsigned int size, const char* mask);
DWORD GetModuleBaseAddr(DWORD processID, const char* moduleName);


//--Templates
//--For these, _addrType is the architecture. x86 pointer size is DWORD (unsigned long), x64 is DWORD64 (unsigned long long)
template <typename _addrType>
void vTableHook(void* classBase, void* hook, unsigned int index)
{
	DWORD oldProtect = 0, newProtect = 0;

	//--32 or 64 bit program?

	_addrType vTable = *(_addrType*)classBase;
	VirtualProtect((_addrType*)vTable + index, sizeof(_addrType), PAGE_READWRITE, &oldProtect);
	*((_addrType*)vTable + index) = (_addrType)hook;
	VirtualProtect((_addrType*)vTable + index, sizeof(_addrType), oldProtect, &newProtect);
}
#endif
template <typename _addrType>
void DumpVTable(void* V, unsigned int depth)
{
	_addrType vTable = *(_addrType*)V;
	if (vTable == NULL)
	{
		printf("VTable is nullptr\n");
		return;
	}
	printf("VTable: %p\n{\n", (void*)vTable);
	for (unsigned int i = 0; i < depth; i++)
	{
		printf("\tFUNC %d: %p\n", i, (void*)*((_addrType*)vTable + i));
	}
	printf("};\n");
}
template<typename _addrType>
_addrType FindDMAAddr(void* startAddr, const _addrType* offsets, unsigned int size)
{
	if (startAddr == NULL)
	{
		return NULL;
	}
	_addrType addr = (_addrType)startAddr;
	for (int i = 0; i < size; i++)
	{
		if (addr == NULL)
		{
			return NULL;
		}
		addr = *(_addrType*)(addr + offsets[i]);
	}
	return addr;
}
