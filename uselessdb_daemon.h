/*
 * uselessdb_daemon.h
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: gorec
 */

#ifndef USELESSDB_DAEMON_H_
#define USELESSDB_DAEMON_H_


#include "uselessnl_core.h"
#include "uselessnl_req_processor.h"

class UselessDBDaemon{
public:
	UselessDBDaemon();
	~UselessDBDaemon();
	int Init();
	void Run();
private:
	int NotifyKernelModule(int daemon_pid);
	UselessNLCore unl_communicator;
	UselessNLReqProcessor req_processor;
	bool init_flag;
};



#endif /* USELESSDB_DAEMON_H_ */
