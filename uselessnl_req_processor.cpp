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

int UselessNLReqProcessor::Init(){
	int ret=db_manager.Init();
	return ret;
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
		SendReportToClient(MSGTYPE_RESP_ERROR,pid);
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
			if(ret>0){
				ret=0;
			}
		}
	}
	else{
		ret=1;
	}
	return ret;
}

int UselessNLReqProcessor::Connect(std::list<std::string> parsed_data, uint32_t pid){
	int ret=1;
	if(parsed_data.size()==1){
		auto u_name=parsed_data.begin();
		if(u_name->length()>0){
			ret=db_manager.Connect(*u_name);
			if(ret==0){
				SendReportToClient(MSGTYPE_RESP_CONNECT,pid);
			}
		}
	}
	return ret;
}

int UselessNLReqProcessor::Disconnect(std::list<std::string> parsed_data, uint32_t pid){
	int ret=1;
	if(parsed_data.size()==1){
		auto u_name=parsed_data.begin();
		if(u_name->length()>0){
			ret=db_manager.Disconnect(*u_name);
			if(ret==0){
				SendReportToClient(MSGTYPE_RESP_DISCONNECT,pid);
			}
		}
	}
	return ret;
}

int UselessNLReqProcessor::Get(std::list<std::string> parsed_data, uint32_t pid){
	int ret=1;
	if(parsed_data.size()==2){
		auto it=parsed_data.begin();
		std::string u_name=*it;
		std::string key=*(++it);
		std::string value;
		if(u_name.length()>0 && key.length()>0){
			ret=db_manager.Get(u_name,key,&value);
			if(ret==0){
				std::string msg_buf;
				ret=UselessProtocolParser::PrepareStr(value,msg_buf);
				if(ret==0){
					ret=UselessProtocolParser::InjectPid(msg_buf,pid);
				}
				if(ret==0){
					UselessNLMsg msg(MSGTYPE_RESP_GET,msg_buf);
					unl_communicator->SendMsg(msg);
				}
			}
		}
	}
	return ret;
}

int UselessNLReqProcessor::Set(std::list<std::string> parsed_data, uint32_t pid){
	int ret=1;
	if(parsed_data.size()==3){
		auto it=parsed_data.begin();
		std::string u_name=*it;
		std::string key=*(++it);
		std::string value=*(++it);
		if(u_name.length()>0 && key.length()>0){
			ret=db_manager.Set(u_name,key,value);
			if(ret==0){
				SendReportToClient(MSGTYPE_RESP_SET,pid);
			}
		}
	}
	return ret;
}

int UselessNLReqProcessor::Remove(std::list<std::string> parsed_data, uint32_t pid){
	int ret=1;
	if(parsed_data.size()==2){
		auto it=parsed_data.begin();
		std::string u_name=*it;
		std::string key=*(++it);
		if(u_name.length()>0 && key.length()>0){
			ret=db_manager.Remove(u_name,key);
			if(ret==0){
				SendReportToClient(MSGTYPE_RESP_REMOVE,pid);
			}
		}
	}
	return ret;
}

int UselessNLReqProcessor::UserAdd(std::list<std::string> parsed_data, uint32_t pid){
	int ret=1;
	if(parsed_data.size()==2){
		auto it=parsed_data.begin();
		std::string u_name=*it;
		std::string new_user=*(++it);
		if(u_name.length()>0 && new_user.length()>0){
			ret=db_manager.UserAdd(u_name,new_user);
			if(ret==0){
				SendReportToClient(MSGTYPE_RESP_USERADD,pid);
			}
		}
	}
	return ret;
}

int UselessNLReqProcessor::UserRemove(std::list<std::string> parsed_data, uint32_t pid){
	int ret=1;
	if(parsed_data.size()==2){
		auto it=parsed_data.begin();
		std::string u_name=*it;
		std::string user_to_del=*(++it);
		if(u_name.length()>0 && user_to_del.length()>0){
			ret=db_manager.UserRemove(u_name,user_to_del);
			if(ret==0){
				SendReportToClient(MSGTYPE_RESP_USERREMOVE,pid);
			}
		}
	}
	return ret;
}

void UselessNLReqProcessor::SendReportToClient(uint16_t msg_type, uint32_t pid){
	int ret=0;
	std::string err_msg_buf;
	ret=UselessProtocolParser::InjectPid(err_msg_buf,pid);
	if(ret==0){
		UselessNLMsg error_msg(msg_type,err_msg_buf);
		unl_communicator->SendMsg(error_msg);
	}
}
