//--Log.h
#pragma once
#include "sLib.h"

class Log
{
public:
	void Write(std::string dialogue, int newLine=1);
	void WriteMem(void* loc, unsigned int size);
	void DumpLocation(const char* path);
	void Dump();
	void Read();
	Log(const char* path, const char* nameOpt="Log");
	~Log();
private:
	std::vector<std::string> logData;
	const char* dumpPath;
	const char* name;
};