#include "OS_ABSTRACTION.h"

void Os_Abstraction::QSLEEP()
{
	std::this_thread::sleep_for(std::chrono::milliseconds(2));
}
void Os_Abstraction::QLOG_INFO(std::string String_out)
{
	std::cout << "OS LEVEL LOG: " << String_out << std::endl;
}


