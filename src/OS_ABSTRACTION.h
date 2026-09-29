#include <iostream>
#include <thread>
#include <chrono>
#include <string>

class Os_Abstraction
{
public:
	static void QSLEEP();
	static void QLOG_INFO(std::string String_out);
};
