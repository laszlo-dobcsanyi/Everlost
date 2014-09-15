#ifndef CORE_LOGGER_H
#define CORE_LOGGER_H

#include <iostream>
#include <string>

class Logger
{
public:
    static void Write(const int& _level, const std::string& _msg);

private:
    static int level;
    static bool only;
	static void* console;

	Logger();
};

#endif