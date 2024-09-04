// Author: Deci | Project: EternalAdvanced | Name: Logging.h
// Date: 2024/08/19 @ 12:08:28

#pragma once
#include <fstream>


inline std::ofstream g_logfile;

inline void InitLog()
{
	g_logfile = std::ofstream("log.txt", std::ios::app);
}

inline void CloseLog()
{
	g_logfile.flush();
	g_logfile.close();
}
