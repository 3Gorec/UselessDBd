/*
 * uselessnl_req_processor.h
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: gorec
 */

#include "uselessnl_core.h"

class UselessNLReqProcessor{
public:
	UselessNLReqProcessor(UselessNLCore *unl_communicator);
	~UselessNLReqProcessor();
	int ProcessRequest(UselessNLMsg &msg);
private:
	UselessNLCore *unl_communicator;
};


