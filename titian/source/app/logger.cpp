#include "titian.h"


bool titian::Logger::m_is_ready = false;
titian::List<titian::LogInfo> titian::Logger::logs = {};
uint64_t titian::Logger::last_log_index = 0;

void titian::Logger::set_ready()
{
	m_is_ready = true;
}
