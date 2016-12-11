/*
 * uselessnl_req_processor.cpp
 *
 *  Created on: 8 дек. 2016 г.
 *      Author: gorec
 */

#include "uselessnl_req_processor.h"
#include "useless_protocol_parser.h"
#include <stdio.h>

UselessNLReqProcessor::UselessNLReqProcessor(UselessNLCore *unl_communicator){
	this->unl_communicator=unl_communicator;
}

UselessNLReqProcessor::~UselessNLReqProcessor(){

}

int UselessNLReqProcessor::ProcessRequest(UselessNLMsg &msg){
	int ret=0;
	std::string msg_buf(msg.data,msg.data_len);
	std::list<std::string>parsed_data;
	uint32_t pid=UselessProtocolParser::WithdrawPid(msg_buf);
	ret=UselessProtocolParser::ParseMsg(msg_buf,parsed_data);
	if(pid!=0 && ret==0){
		switch(msg.type){
			case MSGTYPE_REQ_ECHO:
				ret=Echo(parsed_data,pid);
				break;
			case MSGTYPE_REQ_CONNECT:
				ret=Connect(parsed_data,pid);
				break;
			case MSGTYPE_REQ_DISCONNECT:
				ret=Disconnect(parsed_data,pid);
				break;
			case MSGTYPE_REQ_GET:
				ret=Get(parsed_data,pid);
				break;
			case MSGTYPE_REQ_SET:
				ret=Set(parsed_data,pid);
				break;
			case MSGTYPE_REQ_REMOVE:
				ret=Remove(parsed_data,pid);
				break;
			case MSGTYPE_REQ_USERADD:
				ret=UserAdd(parsed_data,pid);
				break;
			case MSGTYPE_REQ_USERREMOVE:
				ret=UserRemove(parsed_data,pid);
				break;
		}
	}
	if(ret!=0 && pid!=0){
		SendProcessErrorToClient(pid);
	}
	return ret;
}

int UselessNLReqProcessor::Echo(std::list<std::string> parsed_data, uint32_t pid){
	std::string msg_buf;
	auto it=parsed_data.begin();
	int ret=0;
	if(parsed_data.size()==1){
		ret=UselessProtocolParser::PrepareStr(*it,msg_buf);
		ret=UselessProtocolParser::InjectPid(msg_buf,pid);
		if(ret==0){
			UselessNLMsg msg(MSGTYPE_RESP_ECHO,msg_buf);
			ret=unl_communicator->SendMsg(msg);
		}
	}
	else{
		ret=1;
	}
	return ret;
}

int UselessNLReqProcessor::Connect(std::list<std::string> parsed_data, uint32_t pid){

}

int UselessNLReqProcessor::Disconnect(std::list<std::string> parsed_data, uint32_t pid){

}

int UselessNLReqProcessor::Get(std::list<std::string> parsed_data, uint32_t pid){

}

int UselessNLReqProcessor::Set(std::list<std::string> parsed_data, uint32_t pid){

}

int UselessNLReqProcessor::Remove(std::list<std::string> parsed_data, uint32_t pid){

}

int UselessNLReqProcessor::UserAdd(std::list<std::string> parsed_data, uint32_t pid){

}

int UselessNLReqProcessor::UserRemove(std::list<std::string> parsed_data, uint32_t pid){

}

void UselessNLReqProcessor::SendProcessErrorToClient(uint32_t pid){
	int ret=0;
	std::string err_msg_buf;
	ret=UselessProtocolParser::InjectPid(err_msg_buf,pid);
	if(ret==0){
		UselessNLMsg error_msg(MSGTYPE_RESP_ERROR,err_msg_buf);
		unl_communicator->SendMsg(error_msg);
	}
}
