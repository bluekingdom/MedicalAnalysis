#include "glog.h"
#include "Utils.h"

#include <string>
#include <ctime>
#include <sstream>

namespace SYY {


	bool GLog::s_bIsInit = false;

	bool GLog::Init()
	{
		const std::string exePath = Utils::GetCurExePath();

		const std::string logPath = exePath + "\\logs";

		if (false == Utils::CreateDir(logPath))
		{
			return false;
		}

		s_bIsInit = true;

		time_t now; 
		std::time(&now);

		std::stringstream ss;
		ss << logPath + "\\SYY_MedicalAnalysis-" << now << ".log";
		const std::string logFile = ss.str();

		m_ofstream.open(logFile);
		if (false == m_ofstream.is_open())
		{
			return false;
		}

		return true;
	}

	GLog* GLog::GetInstance()
	{
		static GLog g_instance;
		if (false == s_bIsInit && false == g_instance.Init())
			return nullptr;

		return &g_instance;
	}

	GLog::~GLog()
	{
		m_ofstream.close();
	}

	
}