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

int DB_Manager::Set(std::string &key, std::string &value){
	int ret=0;
	char *sql_query;
	sqlite3_stmt *stmt;

	ret=Get(key,0);
	if(ret==0){	//Заменить запись
		ret=RewriteEntry(key,value);
		return ret;
	}
	else if(ret==-1){	//Добавить новую
		sql_query=(char *)"INSERT INTO "ULTABLE_NAME" VALUES (?1,?2);";
	}
	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret==SQLITE_OK){
			sqlite3_bind_text(stmt,1,key.data(),-1,0);
			sqlite3_bind_text(stmt,2,value.data(),-1,0);
	}
	else{
		OutputError();
	}

	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
		if(ret==SQLITE_DONE){
			ret=0;
		}
		else{
			OutputError();
		}
		sqlite3_finalize(stmt);
	}
	return ret;
}

int DB_Manager::Get(std::string &key, std::string *value){
	int ret=0;
	sqlite3_stmt *stmt;
	char * sql_query=(char *)"SELECT * FROM "ULTABLE_NAME" WHERE key=?1;";
	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret==SQLITE_OK){
		sqlite3_bind_text(stmt,1,key.data(),-1,0);
	}
	else{
		OutputError();
	}

	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
		if(ret==SQLITE_ROW){//Есть строка стаким ключом
			if(value!=NULL){
				value->assign((const char *)sqlite3_column_text(stmt, 1));	//Получаем значение value
			}
			ret=0;
		}
		else if(ret==SQLITE_DONE){	//нет записи с таким ключом
			ret=-1;
		}
		else{
			OutputError();
		}
		sqlite3_finalize(stmt);
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
		OutputError();
	}
	return ret;
}

int DB_Manager::RewriteEntry(std::string &key, std::string &value){
	int ret=0;
	char *sql_query;
	sqlite3_stmt *stmt;
	sql_query=(char *)"UPDATE "ULTABLE_NAME" SET value=?1 WHERE key=?2;";
	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret==SQLITE_OK){
		sqlite3_bind_text(stmt,1,value.data(),-1,0);
		sqlite3_bind_text(stmt,2,key.data(),-1,0);
	}
	else{
		OutputError();
	}

	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
		if(ret==SQLITE_DONE){
			ret=0;
		}
		else{
			OutputError();
		}
		sqlite3_finalize(stmt);
	}
	return ret;
}

int DB_Manager::InitDB(){
	int ret=0;
	sqlite3_stmt *stmt;
	const char *sql_query="create table if not exists "ULTABLE_NAME" (" \
						"key varchar("TOSTR(MAX_KEY_STR_LEN)") NOT NULL," \
						"value varchar("TOSTR(MAX_VALUE_STR_LEN)") NOT NULL," \
						"PRIMARY KEY (key)" \
						");";

	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret!=SQLITE_OK){
		OutputError();
	}
	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
			if(ret!=SQLITE_DONE){
				OutputError();
			}
			sqlite3_finalize(stmt);
	}
	return ret;
}

void DB_Manager::OutputError(){
	char *error_msg=(char *)sqlite3_errmsg(db);
	printf("Error %s\n",error_msg);
	sqlite3_free(error_msg);
}



