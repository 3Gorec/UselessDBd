/*
 * tests.cpp
 *
 *  Created on: 10 дек. 2016 г.
 *      Author: gorec
 */


#include "tests.h"
#include "db_manager.h"
#include "uselessdb_daemon.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


bool TestSessionManager(){
	std::string root(ROOT_DB_USER);
	bool ret=false;
	int code=0;
	SessionManager s_manager;

	if(s_manager.IsSessionActive(root)){
		code=1;
		goto exit;
	}
	if(s_manager.StartSession(root)!=0){
		code=2;
		goto exit;
	}
	if(s_manager.StartSession(root)!=1){
		code=3;
		goto exit;
	}
	if(!s_manager.IsSessionActive(root)){
		code=4;
		goto exit;
	}
	if(s_manager.EndSession(root)!=0){
		code=5;
		goto exit;
	}
	if(s_manager.EndSession(root)!=1){
		code=6;
		goto exit;
	}
	if(s_manager.IsSessionActive(root)){
		code=7;
		goto exit;
	}
	if(s_manager.StartSession(root)!=0){
		code=8;
		goto exit;
	}
	if(!s_manager.IsSessionActive(root)){
		code=9;
		goto exit;
	}
	sleep(SESSION_TIMEOUT_S+1);
	if(s_manager.IsSessionActive(root)){
		code=10;
		goto exit;
	}
	if(s_manager.StartSession(root)!=0){
		code=11;
		goto exit;
	}
	if(!s_manager.IsSessionActive(root)){
		code=13;
		goto exit;
	}
	sleep(SESSION_TIMEOUT_S/2);
	if(s_manager.UpdateSession(root)!=0){
		code=14;
		goto exit;
	}
	sleep(SESSION_TIMEOUT_S/2+1);
	if(!s_manager.IsSessionActive(root)){
		code=15;
		goto exit;
	}
	if(s_manager.EndSession(root)!=0){
		code=16;
		goto exit;
	}
	if(s_manager.UpdateSession(root)!=1){
		code=17;
		goto exit;
	}
	if(s_manager.IsSessionActive(root)){
		code=18;
		goto exit;
	}
	ret=true;
exit:
	if(!ret){
		printf("SessionManager test error %d\n",code);
	}
	else{
		printf("SessionManager test success\n");
	}
	return ret;
}

bool TestDBManager(){
	/*
		DB_Manager db_manager;
		std::string key,value;

		key.assign("1 1");
		value.assign("test1");
		ret=db_manager.Set(key,value);
		printf("adding key=%s value=%s ret=%d\n",key.data(),value.data(),ret);

		key.assign("2 2");
		value.assign("test2");
		ret=db_manager.Set(key,value);
		printf("adding key=%s value=%s ret=%d\n",key.data(),value.data(),ret);


		key.assign("1 1");
		ret=db_manager.Get(key,&value);
		printf("Get key=%s value=%s ret=%d\n",key.data(),value.data(),ret);

		key.assign("2 2");
		ret=db_manager.Get(key,&value);
		printf("Get key=%s value=%s ret=%d\n",key.data(),value.data(),ret);

		key.assign("3 3");
		ret=db_manager.Get(key,&value);
		printf("Get key=%s value=%s ret=%d\n",key.data(),value.data(),ret);



		key.assign("2 2");
		value.assign("Updated");
		ret=db_manager.Set(key,value);
		printf("adding key=%s value=%s ret=%d\n",key.data(),value.data(),ret);

		key.assign("2 2");
		ret=db_manager.Get(key,&value);
		printf("Get key=%s value=%s ret=%d\n",key.data(),value.data(),ret);

		//db_manager.Flush();
	*/
		/*
		sqlite3 *db;

		char *error_msg;
		ret=sqlite3_open(ULDBFILE_NAME,&db);
		if(ret){
			printf("Opening db error: %s\n",sqlite3_errmsg(db));
			sqlite3_close(db);
			return EXIT_FAILURE;
		}
		else{
			printf("Opening successfull\n");
		}
		ret=sqlite3_exec(db,"create table if not exists "ULTABLE_NAME" ( " \
				"key varchar("TOSTR(MAX_KEY_STR_LEN)") NOT NULL," \
				"value varchar("TOSTR(MAX_VALUE_STR_LEN)") NOT NULL," \
				"PRIMARY KEY (key)" \
				");",NULL,NULL,&error_msg);
		if(ret){
			printf("Create table "ULTABLE_NAME" error:\n %s\n",error_msg);
			sqlite3_free(error_msg);
			sqlite3_close(db);
			return EXIT_FAILURE;
		}
		else{
			printf("Table created\n");
		}
	*/
}
