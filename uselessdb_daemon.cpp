/*
 * uselessdb_daemon.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: gorec
 */


#include "uselessdb_daemon.h"

UselessDBDaemon::UselessDBDaemon():req_processor(&unl_communicator){
	init_flag=false;
}

UselessDBDaemon::~UselessDBDaemon(){

}

int UselessDBDaemon::Init(){
	int daemon_pid=0, ret=1;
	daemon_pid=unl_communicator.Init(0);
	if(daemon_pid!=0){
		ret=NotifyKernelModule(daemon_pid);
	}
	if(ret==0){
		init_flag=true;
	}
	return ret;
}
#include <stdio.h>
void UselessDBDaemon::Run(){
	int ret=0;
	UselessNLMsg msg(0,NL_MAX_PAYLOAD);
	if(init_flag){
		while(1){
			UselessNLMsg msg(0,NL_MAX_PAYLOAD);
			ret=unl_communicator.RecvMsg(msg);
			if(ret!=-1){
				ret=req_processor.ProcessRequest(msg);
				if(ret!=0){
					//todo send error_msg
				}
			}
		}
	}
}

int UselessDBDaemon::NotifyKernelModule(int daemon_pid){
	int ret=0;
	UselessNLMsg send_msg(MSGTYPE_SET_DPID);
	UselessNLMsg recv_msg(0,NL_MAX_PAYLOAD);
	ret=unl_communicator.SendMsg(send_msg);
	if(ret!=-1){
		unl_communicator.RecvMsg(recv_msg);
	}
	if(ret!=-1){
		if(unl_communicator.GetErrorCode(recv_msg)==ERROR_NO){
			ret=0;
		}
		else{
			ret=-2;
		}
	}
	return ret;
}

