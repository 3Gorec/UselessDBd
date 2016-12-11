/*
 * uselessnl_req_processor.h
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: gorec
 */

#include "uselessnl_core.h"
#include <list>
#include <string>

class UselessNLReqProcessor{
public:
	UselessNLReqProcessor(UselessNLCore *unl_communicator);
	~UselessNLReqProcessor();
	int ProcessRequest(UselessNLMsg &msg);
private:
	int Echo(std::list<std::string> parsed_data, uint32_t pid);
	int Connect(std::list<std::string> parsed_data, uint32_t pid);
	int Disconnect(std::list<std::string> parsed_data, uint32_t pid);
	int Get(std::list<std::string> parsed_data, uint32_t pid);
	int Set(std::list<std::string> parsed_data, uint32_t pid);
	int Remove(std::list<std::string> parsed_data, uint32_t pid);
	int UserAdd(std::list<std::string> parsed_data, uint32_t pid);
	int UserRemove(std::list<std::string> parsed_data, uint32_t pid);
	void SendProcessErrorToClient(uint32_t pid);
	UselessNLCore *unl_communicator;
};


