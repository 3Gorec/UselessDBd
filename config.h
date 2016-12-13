/*
 * config.h
 *
 *  Created on: 30 нояб. 2016 г.
 *      Author: gorec
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define TOSTR2(X) #X
#define TOSTR(X) TOSTR2(X)

#define ULDBFILE_NAME		"/tmp/db"
#define DATA_TABLE_NAME		"useless_data"
#define USER_TABLE_NAME		"useless_users"
#define MAX_USER_STR_LEN	200
#define MAX_KEY_STR_LEN		200
#define MAX_VALUE_STR_LEN	200
#define ROOT_DB_USER		"root"

#define SESSION_TIMEOUT_S	3600

#endif /* CONFIG_H_ */
