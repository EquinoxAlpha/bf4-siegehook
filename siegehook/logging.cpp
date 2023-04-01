#include "logging.h"

void logging::log(const char* format, ...)
{
	return;
	printf("[+] ");
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

void logging::error(const char* format, ...) {
	printf("[!] ");
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);
}

logging logger;