#include "main.h"


bool titian::Logger::m_is_ready = false;
std::list<titian::LogInfo> titian::Logger::logs = {};
uint64_t titian::Logger::last_log_index = 0;

void titian::Logger::set_ready()
{
	m_is_ready = true;
}
