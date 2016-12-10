/*
 * db_manager.h
 *
 *  Created on: 30 нояб. 2016 г.
 *      Author: gorec
 */

#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include "config.h"
#include "session_manager.h"
#include <string>
#include <sqlite3.h>

class DB_Manager{
public:
	DB_Manager();
	~DB_Manager();
	int Init();
	int Connect(std::string &user);
	int Disconnect(std::string &user);
	int Set(std::string &user, std::string &key, std::string &value);
	int Get(std::string &user, std::string &key, std::string *value);
	int Remove(std::string &user, std::string key);
	int FlushDataTable();
	int UserAdd(std::string &user, std::string &new_user);
	int UserRemove(std::string &user, std::string &user_to_delete);
private:
	sqlite3 *db;
	SessionManager session_mngr;
	bool UserExist(std::string &user);
	int RewriteEntry(std::string &key, std::string &value);
	int InsertEntryUserTable(std::string &user_name);
	int InitDataTable();
	int InitUserTable();
	void OutputSqliteError();
};

#endif /* DB_MANAGER_H_ */
