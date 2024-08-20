// Author: Deci | Project: DEAO2 | Name: Logging.h
// Date: 2024/08/19 @ 12:08:28

#pragma once
#include <fstream>


inline std::ofstream g_logfile;

inline void init_log()
{
	g_logfile = std::ofstream("log.txt", std::ios::app);
}

inline void close_log()
{
	g_logfile.flush();
	g_logfile.close();
}
