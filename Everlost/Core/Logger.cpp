#include "Core\Macro.h"
#include "Core\Logger.h"

#include "Windows.h"

int Logger::level = 9;
bool Logger::only = false;
HANDLE Logger::console = GetStdHandle(STD_OUTPUT_HANDLE);

void Logger::Write(const int& _level, const std::string& _msg)
{
	//bit 0 - foreground blue
	//bit 1 - foreground green
	//bit 2 - foreground red
	//bit 3 - foreground intensity

	//bit 4 - background blue
	//bit 5 - background green
	//bit 6 - background red
	//bit 7 - background intensity

	static const WORD colors[] = {  0x0A,
									0x0C, 0x04, 0x04,
									0x0A, 0x02, 0x02,
									0x0B, 0x03, 0x03};
	if (((!only) && (_level <= level)) || ((only) && (_level == level)))
	{
		SetConsoleTextAttribute(console, ((0 <= _level) &&(_level <= 9)) ? colors[_level] : 0x0F);
		std::cout << _msg.c_str() << std::endl;
	}
}
