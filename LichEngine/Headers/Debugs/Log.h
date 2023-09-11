#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <vector>

#include <Define.h>


namespace Debug
{
	enum LogLevel
	{
		LOG_LEVEL_FATAL = 0,
		LOG_LEVEL_ERROR = 1,
		LOG_LEVEL_WARNING = 2,
		LOG_LEVEL_INFO = 3,
		LOG_LEVEL_DEBUG = 4
	};

#define LOG_WARNING_ENABLE 1 
#define LOG_INFO_ENABLE 1 
#define LOG_DEBUG_ENABLE 1

#ifdef LRELAESE
#define LOG_WARNING_ENABLE  0;
#define LOG_DEBUG_ENABLE  0;
#endif 

#ifdef LASSERTION
#define LINFO_ASSERTION(expression,message) {std::string msg = message; Debug::Log::LogMsg(Debug::LogLevel::LOG_LEVEL_FATAL,expression + msg,__LINE__,__FILE__,__FUNCTION__);}
#endif // LASSERTION

#define LFATAL(message) Debug::Log::LogMsg(Debug::LogLevel::LOG_LEVEL_FATAL,message,__LINE__,__FILE__,__FUNCTION__)
#define LERROR(message) Debug::Log::LogMsg(Debug::LogLevel::LOG_LEVEL_ERROR,message,__LINE__,__FILE__,__FUNCTION__)

#if LOG_WARNING_ENABLE == 1
#define LWARNING(message) Debug::Log::LogMsg(Debug::LogLevel::LOG_LEVEL_WARNING,message,__LINE__,__FILE__,__FUNCTION__)
#else
#define LWARNING(message) 
#endif 

#if LOG_INFO_ENABLE == 1
#define LINFO(message) Debug::Log::LogMsg(Debug::LogLevel::LOG_LEVEL_INFO,message,__LINE__,__FILE__,__FUNCTION__)
#else
#define LINFO(message) 
#endif 

#if LOG_DEBUG_ENABLE == 1
#define LDEBUG(message) Debug::Log::LogMsg(Debug::LogLevel::LOG_LEVEL_DEBUG,message,__LINE__,__FILE__,__FUNCTION__)
#else
#define LDEBUG(message) 
#endif

	static class LICH_ENGINE Log
	{
	public:
		static std::string logPath;
		static void SetupLog();
		static void ShutdownLog();
		static void LogMsg(LogLevel level,std::string message,unsigned int line,const std::string fileName,const std::string functionName);
		static void ClearLogArray();
		static char* logArray[100];
		static unsigned int logIndex;
	private:
		static std::ofstream logFile;
	};
}