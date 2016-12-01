/*
 * db_manager.cpp
 *
 *  Created on: 30 нояб. 2016 г.
 *      Author: gorec
 */

#include "db_manager.h"
#include <stdio.h>	//todo remove

DB_Manager::DB_Manager(){
	int ret;
	ret=sqlite3_open(ULDBFILE_NAME,&db);
	if(ret){
		//todo process error;
	}
	if(ret==0){
		ret=InitDB();
	}
	if(ret==0){
		//todo log
	}
}

DB_Manager::~DB_Manager(){
	sqlite3_close(db);
}

int DB_Manager::Set(std::string key, std::string &value){
	int ret=0;
	std::string set_query;
	char *error_msg;
	ret=Get(key,0);
	if(ret==-1){	//Добавить новую
		set_query.assign("INSERT INTO "ULTABLE_NAME" VALUES ('");
		set_query.append(key);
		set_query.append("','");
		set_query.append(value);
		set_query.append("');");
	}
	else if(ret==0){	//Заменить запись
		set_query.assign("UPDATE "ULTABLE_NAME" SET value='");
		set_query.append(value);
		set_query.append("' WHERE key='");
		set_query.append(key);
		set_query.append("';");
	}
	ret=sqlite3_exec(db,set_query.data(),NULL,NULL,&error_msg);
	if(ret!=0){
		//todo process error
	}
	return ret;
}

int DB_Manager::Get(std::string key, std::string *value){
	int ret=0;
	char *error_msg;
	std::string get_query="SELECT * FROM "ULTABLE_NAME" WHERE key='";
	get_query.append(key);
	get_query.append("';");
	char **results = NULL;
	int rows, columns;
	ret=sqlite3_get_table(db,get_query.data(),&results, &rows, &columns, &error_msg);
	if(ret==0){
		if(rows==1){	// ровно одна запись с уникальным ключом
			if(value!=NULL){
				value->assign(results[((rows+1)*columns)-1]);	//Получаем значение value
			}
		}
		else{
			ret=-1;
		}
		sqlite3_free_table(results);
	}
	else{
		//todo process error
	}
	return ret;
}

int DB_Manager::Remove(std::string key){
	int ret=0;

	return ret;
}

int DB_Manager::Flush(){
	int ret=0;
	char *error_msg;
	ret=sqlite3_exec(db,"DELETE FROM "ULTABLE_NAME";",NULL,NULL,&error_msg);
	if(ret){
		//printf("Create table "ULTABLE_NAME" error:\n %s\n",error_msg);	todo process error
		//sqlite3_free(error_msg);
		//sqlite3_close(db);
		//return EXIT_FAILURE;
	}
	return ret;
}

int DB_Manager::InitDB(){
	int ret=0;
	char *error_msg;
	ret=sqlite3_exec(db,"create table if not exists "ULTABLE_NAME" ( " \
	"key varchar("TOSTR(MAX_KEY_STR_LEN)") NOT NULL," \
	"value varchar("TOSTR(MAX_VALUE_STR_LEN)") NOT NULL," \
	"PRIMARY KEY (key)" \
	");",NULL,NULL,&error_msg);
	if(ret){
		//printf("Create table "ULTABLE_NAME" error:\n %s\n",error_msg);	todo process error
		//sqlite3_free(error_msg);
		//sqlite3_close(db);
		//return EXIT_FAILURE;
	}

	return 0;
}





