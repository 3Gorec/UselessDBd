/*
 * db_manager.h
 *
 *  Created on: 30 нояб. 2016 г.
 *      Author: gorec
 */

#ifndef DB_MANAGER_H_
#define DB_MANAGER_H_

#include "config.h"
#include <string>
#include <sqlite3.h>

class DB_Manager{
public:
	DB_Manager();
	~DB_Manager();
	int Set(std::string key, std::string &value);
	int Get(std::string key, std::string *value);
	int Remove(std::string key);
	int Flush();
private:
	sqlite3 *db;
	int InitDB();
};

#endif /* DB_MANAGER_H_ */
