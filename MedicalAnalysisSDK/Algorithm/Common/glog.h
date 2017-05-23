#pragma once

#include <ctime>
#include <fstream>
#include <stdio.h>
#include <sstream>

namespace SYY
{
#define GLOG(mode, ...) \
	GLog::GetInstance()->Log(__FILE__, __LINE__, __FUNCTION__, mode, __VA_ARGS__)

	class GLog {
	public:
		static GLog* GetInstance();

		bool Init();

		template <typename... TS>
		bool Log(const char* filename, int line, const char* funcname,
			const char* mode, TS... args);

	private:
		static bool s_bIsInit;
		std::ofstream m_ofstream;

	private:
		GLog() {}
		GLog(GLog&) {}
		~GLog();
	};

	template <typename... TS>
	bool SYY::GLog::Log(const char* filename, int line_no, const char* funcname, const char* mode, TS... args)
	{
		time_t now; 
		std::time(&now);
		tm *ptm = localtime(&now);

		std::stringstream ss;
		ss.fill('0');
		ss << "[ ";
		ss.width(4);
		ss << 1900 + ptm->tm_year;
		ss << "-";
		ss.width(2);
		ss << 1 + ptm->tm_mon;
		ss << "-";
		ss.width(2);
		ss << ptm->tm_mday;
		ss << " ";
		ss.width(2);
		ss << ptm->tm_hour;
		ss << ":";
		ss.width(2);
		ss << ptm->tm_min;
		ss << ":";
		ss.width(2);
		ss << ptm->tm_sec;
		ss << " <FILE: " << filename << "> <FUNC: " << funcname << "> LINE:" << line_no;
		ss << " ] ";

		ss << mode;

		char line[65535];
		sprintf(line, ss.str().c_str(), args...);
		m_ofstream << line;
		m_ofstream.flush();

		return true;
	}

}
