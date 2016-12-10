/*
 * db_manager.cpp
 *
 *  Created on: 30 нояб. 2016 г.
 *      Author: gorec
 */

#include "db_manager.h"
#include <stdio.h>	//todo remove
#include <assert.h>

DB_Manager::DB_Manager(){

}

DB_Manager::~DB_Manager(){
	sqlite3_close(db);
}

int DB_Manager::Init(){
	int ret;
	ret=sqlite3_open(ULDBFILE_NAME,&db);
	if(ret!=0){
		printf("Sqlite file '%s' open error\n",ULDBFILE_NAME);
	}
	if(ret==0){
		ret=InitDataTable();
		if(ret!=0){
			printf("Data table init error\n");
		}
	}
	if(ret==0){
		ret=InitUserTable();
		if(ret!=0){
			printf("Data table init error\n");
		}
	}
	return ret;
}

int DB_Manager::Connect(std::string &user){
	int ret=1;
	if(!session_mngr.IsSessionActive(user)){
		if(UserExist(user)){
			ret=session_mngr.StartSession(user);
		}
	}
	return ret;
}

int DB_Manager::Disconnect(std::string &user){
	int ret=0;
	if(session_mngr.IsSessionActive(user)){
		ret=session_mngr.EndSession(user);
	}
	return ret;
}

int DB_Manager::Set(std::string &user, std::string &key, std::string &value){
	int ret=0;
	char *sql_query;
	sqlite3_stmt *stmt;

	assert(user.length()<MAX_USER_STR_LEN);
	assert(key.length()<MAX_KEY_STR_LEN);
	assert(value.length()<MAX_VALUE_STR_LEN);

	ret=session_mngr.CheckNUpdateSession(user);
	if(ret==0){
		ret=Get(user,key,0);
		if(ret==0){	//Заменить запись
			ret=RewriteEntry(key,value);
			return ret;
		}
		else if(ret==-1){	//Добавить новую
			sql_query=(char *)"INSERT INTO "DATA_TABLE_NAME" VALUES (?1,?2);";
		}
		ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
		if(ret==SQLITE_OK){
				sqlite3_bind_text(stmt,1,key.data(),-1,0);
				sqlite3_bind_text(stmt,2,value.data(),-1,0);
		}
		else{
			OutputSqliteError();
		}

		if(ret==SQLITE_OK){
			ret=sqlite3_step(stmt);
			if(ret==SQLITE_DONE){
				ret=0;
			}
			else{
				OutputSqliteError();
			}
			sqlite3_finalize(stmt);
		}
	}
	return ret;
}

int DB_Manager::Get(std::string &user, std::string &key, std::string *value){
	int ret=0;
	sqlite3_stmt *stmt;
	char * sql_query=(char *)"SELECT * FROM "DATA_TABLE_NAME" WHERE key=?1;";
	assert(user.length()<MAX_USER_STR_LEN);
	assert(key.length()<MAX_KEY_STR_LEN);

	ret=session_mngr.CheckNUpdateSession(user);
	if(ret==0){
		ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
		if(ret==SQLITE_OK){
			sqlite3_bind_text(stmt,1,key.data(),-1,0);
		}
		else{
			OutputSqliteError();
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
				OutputSqliteError();
			}
			sqlite3_finalize(stmt);
		}
	}
	return ret;
}

int DB_Manager::Remove(std::string &user, std::string key){
	int ret=0;
	sqlite3_stmt *stmt;
	char * sql_query=(char *)("DELETE FROM "DATA_TABLE_NAME" WHERE key=?1;");

	assert(user.length()<MAX_USER_STR_LEN);
	assert(key.length()<MAX_KEY_STR_LEN);

	ret=session_mngr.CheckNUpdateSession(user);
	if(ret==0){
		ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
		if(ret==SQLITE_OK){
			sqlite3_bind_text(stmt,1,key.data(),-1,0);
		}
		else{
			OutputSqliteError();
		}
		if(ret==SQLITE_OK){
			ret=sqlite3_step(stmt);
			if(ret==SQLITE_DONE){
				ret=0;
			}
			else{
				OutputSqliteError();
			}
			sqlite3_finalize(stmt);
		}
	}
	return ret;
}

int DB_Manager::FlushDataTable(){
	int ret=0;
	char *error_msg;
	ret=sqlite3_exec(db,"DELETE FROM "DATA_TABLE_NAME";",NULL,NULL,&error_msg);
	if(ret){
		OutputSqliteError();
	}
	return ret;
}

int DB_Manager::UserAdd(std::string &user, std::string &new_user){
	int ret=0;

	assert(user.length()<MAX_USER_STR_LEN);

	ret=session_mngr.CheckNUpdateSession(user);
	if(ret==0){
		if(!UserExist(user)){
			ret=InsertEntryUserTable(new_user);
		}
	}
	return ret;
}

int DB_Manager::UserRemove(std::string &user, std::string &user_to_delete){
	int ret=0;
	sqlite3_stmt *stmt;
	char * sql_query=(char *)("DELETE FROM "USER_TABLE_NAME" WHERE key=?1;");

	assert(user.length()<MAX_USER_STR_LEN);
	ret=session_mngr.CheckNUpdateSession(user);
	if(ret==0){
		if(UserExist(user_to_delete)){
			ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
			if(ret==SQLITE_OK){
				sqlite3_bind_text(stmt,1,user_to_delete.data(),-1,0);
			}
			else{
				OutputSqliteError();
			}
			if(ret==SQLITE_OK){
				ret=sqlite3_step(stmt);
				if(ret==SQLITE_DONE){
					ret=0;
				}
				else{
					OutputSqliteError();
				}
				sqlite3_finalize(stmt);
			}
		}
	}
	return ret;
}

bool DB_Manager::UserExist(std::string &user){
	bool exist=false;
	int ret=0;
	sqlite3_stmt *stmt;

	assert(user.length()<MAX_USER_STR_LEN);

	char * sql_query=(char *)"SELECT * FROM "USER_TABLE_NAME" WHERE user=?1;";
	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret==SQLITE_OK){
		sqlite3_bind_text(stmt,1,user.data(),-1,0);
	}
	else{
		OutputSqliteError();
	}

	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
		if(ret==SQLITE_ROW){//Есть строка стаким ключом
			exist=true;
		}
		else if(ret==SQLITE_DONE){	//нет записи с таким ключом
			ret=-1;
		}
		else{
			OutputSqliteError();
		}
		sqlite3_finalize(stmt);
	}
	if(ret!=0){
		exist=false;
	}
	return exist;
}

int DB_Manager::RewriteEntry(std::string &key, std::string &value){
	int ret=0;
	char *sql_query;
	sqlite3_stmt *stmt;
	sql_query=(char *)"UPDATE "DATA_TABLE_NAME" SET value=?1 WHERE key=?2;";
	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret==SQLITE_OK){
		sqlite3_bind_text(stmt,1,value.data(),-1,0);
		sqlite3_bind_text(stmt,2,key.data(),-1,0);
	}
	else{
		OutputSqliteError();
	}

	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
		if(ret==SQLITE_DONE){
			ret=0;
		}
		else{
			OutputSqliteError();
		}
		sqlite3_finalize(stmt);
	}
	return ret;
}

int DB_Manager::InitDataTable(){
	int ret=0;
	sqlite3_stmt *stmt;
	const char *sql_query="create table if not exists "DATA_TABLE_NAME" (" \
						"key varchar("TOSTR(MAX_KEY_STR_LEN)") NOT NULL," \
						"value varchar("TOSTR(MAX_VALUE_STR_LEN)") NOT NULL," \
						"PRIMARY KEY (key)" \
						");";

	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret!=SQLITE_OK){
		OutputSqliteError();
	}
	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
			if(ret!=SQLITE_DONE){
				OutputSqliteError();
			}
			sqlite3_finalize(stmt);
	}
	return ret;
}

int DB_Manager::InitUserTable(){
	int ret=0;
	std::string default_user(ROOT_DB_USER);
	sqlite3_stmt *stmt;
	const char *sql_query="create table if not exists "USER_TABLE_NAME" (" \
						"user varchar("TOSTR(MAX_KEY_STR_LEN)") NOT NULL," \
						"PRIMARY KEY (user)" \
						");";

	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret!=SQLITE_OK){
		OutputSqliteError();
	}
	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
			if(ret!=SQLITE_DONE){
				OutputSqliteError();
			}
			sqlite3_finalize(stmt);
	}
	if(ret==SQLITE_DONE){
		if(!UserExist(default_user)){
			InsertEntryUserTable(default_user);
		}
	}

	return ret;
}

int DB_Manager::InsertEntryUserTable(std::string &user_name){
	int ret=0;
	char *sql_query;
	sqlite3_stmt *stmt;

	sql_query=(char *)"INSERT INTO "USER_TABLE_NAME" VALUES (?1);";
	ret=sqlite3_prepare_v2(db, sql_query, -1, &stmt, 0);
	if(ret==SQLITE_OK){
			sqlite3_bind_text(stmt,1,user_name.data(),-1,0);
	}
	else{
		OutputSqliteError();
	}

	if(ret==SQLITE_OK){
		ret=sqlite3_step(stmt);
		if(ret==SQLITE_DONE){
			ret=0;
		}
		else{
			OutputSqliteError();
		}
		sqlite3_finalize(stmt);
	}
	return ret;
}

void DB_Manager::OutputSqliteError(){
	char *error_msg=(char *)sqlite3_errmsg(db);
	printf("Error %s\n",error_msg);
	sqlite3_free(error_msg);
}



