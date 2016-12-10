/*
 * session_manager.cpp
 *
 *  Created on: 10 дек. 2016 г.
 *      Author: gorec
 */


#include "session_manager.h"
#include "config.h"

SessionManager::SessionManager(){

}

SessionManager::~SessionManager(){

}

int SessionManager::StartSession(std::string &user){
	int ret=0;
	for(auto &it: active_sessions){
		if(it.user==user){
			ret=1;
			break;

		}
	}
	if(ret==0){
		tSession new_session={
			.user=user,
			.timestamp=time(0)
		};
		active_sessions.push_back(new_session);
	}
	return ret;
}

int SessionManager::EndSession(std::string &user){
	int ret=1;
	for(auto &it : active_sessions){
		if(it.user==user){
			active_sessions.remove(it);
			ret=0;
			break;
		}
	}
	return ret;
}

bool SessionManager::IsSessionActive(std::string &user){
	int ret=false;
	for(auto &it: active_sessions){
		if(it.user==user){
			if(time(0)-it.timestamp<=SESSION_TIMEOUT_S){
				ret=true;
			}
			else{
				EndSession(user);
			}
			break;
		}
	}
	return ret;
}

int SessionManager::UpdateSession(std::string &user){
	int ret=1;
	for(auto &it: active_sessions){
		if(it.user==user){
			it.timestamp=time(0);
			ret=0;
			break;
		}
	}
	return ret;
}

int SessionManager::CheckNUpdateSession(std::string &user){
	int ret=1;
	if(IsSessionActive(user)){
		ret=UpdateSession(user);
	}
	return ret;
}
