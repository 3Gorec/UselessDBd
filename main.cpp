//============================================================================
// Name        : UselessDBd.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include "db_manager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(void) {
	int ret=0;
	DB_Manager db_manager;
	std::string key,value;

	/*
	key.assign("1 1");
	value.assign("updated");
	ret=db_manager.Set(key,value);
	printf("adding key=%s value=%s ret=%d\n",key.data(),value.data(),ret);
	key.assign("2 2");
	value.assign("updated2");
	ret=db_manager.Set(key,value);
	printf("adding key=%s value=%s ret=%d\n",key.data(),value.data(),ret);
	 */
	db_manager.Flush();

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



	return EXIT_SUCCESS;
}
