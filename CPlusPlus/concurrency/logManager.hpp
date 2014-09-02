#ifndef __YISIS_LOGMANAGER_HPP_INCLUDED_
#define __YISIS_LOGMANAGER_HPP_INCLUDED_

#include "BlockingQueue.h"
#include <string.h>
#include <iostream>

class LogManager
{
public:
	LogManager(BlockingQueue<std::string>& queue) : queue_(queue) {}
	~LogManager(){}
	int get_message();
	int fetch_to_kafka();
private:
	BlockingQueue<std::string>& queue_;
	std::string message_;
};

void log_fetch_kafka(void *arg);



#endif

