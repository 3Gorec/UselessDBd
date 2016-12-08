/*
 * uselessnl_req_processor.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: gorec
 */

#include "uselessnl_req_processor.h"
#include <stdio.h>

UselessNLReqProcessor::UselessNLReqProcessor(UselessNLCore *unl_communicator){
	this->unl_communicator=unl_communicator;
}

UselessNLReqProcessor::~UselessNLReqProcessor(){

}

int UselessNLReqProcessor::ProcessRequest(UselessNLMsg &msg){
	int ret=0;
	switch(msg.type){
		case MSGTYPE_REQ_ECHO:
			msg.type=MSGTYPE_RESP_ECHO;
			unl_communicator->SendMsg(msg);
			printf("ECHO RECEIVED\n");
			break;

	}
	return ret;
}

