/*
 * session_manager.h
 *
 *  Created on: 10 дек. 2016 г.
 *      Author: gorec
 */

#ifndef SESSION_MANAGER_H_
#define SESSION_MANAGER_H_

#include <time.h>
#include <string>
#include <list>
#include <tuple>

class SessionManager{
public:
	SessionManager();
	~SessionManager();
	int StartSession(std::string &user);
	int EndSession(std::string &user);
	bool IsSessionActive(std::string &user);
	int UpdateSession(std::string &user);
private:
	struct tSession{
		std::string user;
		time_t timestamp;
		bool operator==(const tSession& rhs){
			return std::tie(user, timestamp) == std::tie(rhs.user, rhs.timestamp);
		}
	};
	std::list<tSession> active_sessions;
};



#endif /* SESSION_MANAGER_H_ */
