//--Log.cpp
#include "Log.h"

void Log::Write(std::string dialogue, int newLine)
{
	newLine == 0 ? logData.push_back(dialogue) : logData.push_back(dialogue + '\n');
}

void Log::WriteMem(void* loc, unsigned int size)
{
#if defined(_WIN32)
	DWORD oldProtect, newProtect;
	VirtualProtect(loc, size, PAGE_EXECUTE_READWRITE, &oldProtect);
	logData.push_back(std::string((char*)loc, size));
	VirtualProtect(loc, size, oldProtect, &newProtect);
#endif
}
void Log::DumpLocation(const char* path)
{
	dumpPath = path;
}

void Log::Dump()
{
	if (logData.size() == 0)
	{
		DEBUG_OUT(name << " is empty, Dump() returned void.\n");
		return;
	}
	std::ofstream logOut(dumpPath, std::ofstream::binary);
	for (int i = 0; i < logData.size(); i++)
	{
		std::string tmp = logData[i];
		logOut.write(tmp.c_str(), tmp.size());
	}
	logOut.close();
	logData.clear();
}

void Log::Read()
{
	if (logData.size() == 0)
	{
		DEBUG_OUT(name << " is empty, Read() returned void.\n");
		return;
	}
	for (int i = 0; i < logData.size(); i++)
	{
		std::cout << logData[i];
	}
}

Log::Log(const char* path, const char* nameOpt)
{
	DumpLocation(path);
	name = nameOpt;
}
Log::~Log() = default;