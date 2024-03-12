#pragma once
#include "sLib.h"

#if defined(_WIN32)
void* SignatureScan(char* signature, unsigned int size);
DWORD GetModuleBaseAddr(DWORD processID, const char* moduleName);


//--Templates
//--For these, addrtype is the architecture. x86 pointer size is DWORD (unsigned long), x64 is DWORD64 (unsigned long long)
template <typename addrtype>
void vTableHook(void* classBase, void* hook, unsigned int index)
{
	DWORD oldProtect = 0, newProtect = 0;

	//--32 or 64 bit program?

	addrtype vTable = *(addrtype*)classBase;
	VirtualProtect((addrtype*)vTable + index, sizeof(addrtype), PAGE_READWRITE, &oldProtect);
	*((addrtype*)vTable + index) = (addrtype)hook;
	VirtualProtect((addrtype*)vTable + index, sizeof(addrtype), oldProtect, &newProtect);
}
#endif
template <typename addrtype>
void DumpVTable(void* V, unsigned int depth)
{
	addrtype vTable = *(addrtype*)V;
	if (vTable == NULL)
	{
		printf("VTable is nullptr\n");
		return;
	}
	printf("VTable: %p\n{\n", (void*)vTable);
	for (unsigned int i = 0; i < depth; i++)
	{
		printf("\tFUNC %d: %p\n", i, (void*)*((addrtype*)vTable + i));
	}
	printf("};\n");
}
template<typename addrtype>
addrtype FindDMAAddr(void* startAddr, const addrtype* offsets, unsigned int size)
{
	if (startAddr == NULL)
	{
		return NULL;
	}
	addrtype addr = (addrtype)startAddr;
	for (int i = 0; i < size; i++)
	{
		if (addr == NULL)
		{
			return NULL;
		}
		addr = *(addrtype*)(addr + offsets[i]);
	}
	return addr;
}
