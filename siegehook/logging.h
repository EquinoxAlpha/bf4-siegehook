#pragma once
#include <cstdio>
#include <cstdarg>

class logging
{
public:
	void log(const char* format, ...);
	void error(const char* format, ...);
};

extern logging logger;