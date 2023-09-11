#include <pch.h>
#include <Debugs/Log.h>
#include <Windows.h>
#include <string>
#include <stdio.h>
#include <stdlib.h>

std::ofstream Debug::Log::logFile;
std::string  Debug::Log::logPath = "Log.txt";
unsigned int  Debug::Log::logIndex = 0;
char*  Debug::Log::logArray[100];
void Debug::Log::SetupLog()
{
	if (logFile && logFile.is_open())
	{
		LINFO("Log file already open");
		return;
	}

	logFile.open(Log::logPath,std::ios::out);

	if (!logFile)
		LWARNING("Fail to open log file " + Log::logPath + "\n");
	else
		LINFO("Open log file " + Log::logPath + "\n");
}

void Debug::Log::ShutdownLog()
{
	LINFO("Close log file \n");
	logFile.close();
}

void Debug::Log::LogMsg(LogLevel level,std::string message,unsigned int line,const std::string fileName,const std::string functionName)
{
	const char* level_string[5] = { "[FATAL]: ", "[ERROR]: ","[WARNING]: ", "[INFO]: ","[DEBUG]: " };
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	if (level < 2) SetConsoleTextAttribute(handle,4);

	std::string log = level_string[level];
	const std::string lineC = std::to_string(line);
	const size_t pos = fileName.find_last_of("\\");
	std::string file = fileName.substr(pos + 1,fileName.length() - (pos + 1)).c_str();
	std::cout << log;
	logFile << log;

	SetConsoleTextAttribute(handle,15);

	log = file + "(" + lineC + "): " + message;
	std::cout << log << std::endl;
	logFile << log << std::endl;

	if (level == 3) return;

	// Fill log array
	log = level_string[level] + file + "(" + lineC + "): " + message;

	size_t length = strlen(log.c_str()) + 1;
	logArray[logIndex] = new char[length];
	strcpy_s(logArray[logIndex], length, log.c_str());
	logIndex++;
	if(logIndex==100)
		logIndex = 0;
}

void Debug::Log::ClearLogArray()
{
	for (size_t i = 0; i < 100; i++)
	{
		logArray[i] = new char[0];
	}
	logIndex = 0;
}