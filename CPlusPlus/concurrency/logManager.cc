#include "logManager.hpp"
#include <iostream>
#include <string.h>
using namespace std;

int LogManager::get_message()
{
	message_ = queue_.take();
}

int LogManager::fetch_to_kafka()
{
	while(1) {
		get_message();
		//todo call the kafra c++ client func and send the log out 
	}
}


void log_fetch_kafra(void *arg)
{
	BlockingQueue<std::string>* queue = static_cast<BlockingQueue<std::string>*>(arg);
	LogManager logfetch(*queue);
	logfetch.fetch_to_kafka();
}
