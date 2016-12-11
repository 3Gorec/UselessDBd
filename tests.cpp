/*
 * tests.cpp
 *
 *  Created on: 10 дек. 2016 г.
 *      Author: gorec
 */


#include "tests.h"
#include "db_manager.h"
#include "uselessdb_daemon.h"
#include "useless_protocol_parser.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool TestDBManagerUsers(DB_Manager &db_manager);
static bool TestDBManagerData(DB_Manager &db_manager);

bool TestSessionManager(){
	std::string root(ROOT_DB_USER);
	bool ret=false;
	int code=0;
	SessionManager s_manager;

	//create|active tests
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

	//Update tests
	if(s_manager.StartSession(root)!=0){
		code=11;
		goto exit;
	}
	if(!s_manager.IsSessionActive(root)){
		code=12;
		goto exit;
	}
	sleep(SESSION_TIMEOUT_S/2);
	if(s_manager.UpdateSession(root)!=0){
		code=13;
		goto exit;
	}
	sleep(SESSION_TIMEOUT_S/2+1);
	if(!s_manager.IsSessionActive(root)){
		code=14;
		goto exit;
	}
	if(s_manager.EndSession(root)!=0){
		code=15;
		goto exit;
	}
	if(s_manager.UpdateSession(root)!=1){
		code=16;
		goto exit;
	}
	if(s_manager.IsSessionActive(root)){
		code=17;
		goto exit;
	}

	//CheckNUpdate test
	if(s_manager.StartSession(root)!=0){
		code=18;
		goto exit;
	}
	sleep(SESSION_TIMEOUT_S/2);
	if(s_manager.CheckNUpdateSession(root)!=0){
		code=19;
		goto exit;
	}
	sleep(SESSION_TIMEOUT_S/2+1);
	if(!s_manager.IsSessionActive(root)){
		code=20;
		goto exit;
	}
	if(s_manager.EndSession(root)!=0){
		code=21;
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
	bool test_result;
	DB_Manager db_manager;
	int ret=db_manager.Init();
	if(ret==0){
		test_result=TestDBManagerUsers(db_manager);
		if(test_result){
			test_result=TestDBManagerData(db_manager);
		}
	}
	else{
		printf("DB_manager init error\n");
	}
	return test_result;
}

static bool TestDBManagerUsers(DB_Manager &db_manager){
	bool ret=false;
	int code=0;
	std::string root(ROOT_DB_USER);
	std::string test_usr("test_user");
	std::string test_usr2("test_user2");

	//Root unconnected tests
	if(db_manager.UserAdd(root,test_usr)==0){
		code=1;
		goto exit;
	}
	if(db_manager.UserRemove(root,test_usr)==0){
		code=2;
		goto exit;
	}

	//Root connected tests
	if(db_manager.Connect(root)!=0){
		code=3;
		goto exit;
	}
	if(db_manager.Connect(root)==0){
		code=4;
		goto exit;
	}
	if(db_manager.UserAdd(root,test_usr)!=0){
		code=5;
		goto exit;
	}
	if(db_manager.UserAdd(root,test_usr)==0){
		code=6;
		goto exit;
	}
	if(db_manager.UserRemove(root,test_usr)!=0){
		code=7;
		goto exit;
	}
	if(db_manager.UserRemove(root,test_usr)==0){
		code=8;
		goto exit;
	}
	if(db_manager.UserRemove(root,root)==0){
		code=9;
		goto exit;
	}
	if(db_manager.Disconnect(root)!=0){
		code=10;
		goto exit;
	}
	if(db_manager.Disconnect(root)==0){
		code=11;
		goto exit;
	}

	//test user connected
	if(db_manager.Connect(root)!=0){
		code=12;
		goto exit;
	}
	if(db_manager.UserAdd(root,test_usr)!=0){
		code=13;
		goto exit;
	}
	if(db_manager.Disconnect(root)!=0){
		code=14;
		goto exit;
	}
	if(db_manager.Connect(test_usr)!=0){
		code=15;
		goto exit;
	}
	if(db_manager.UserAdd(test_usr,test_usr2)!=0){
		code=16;
		goto exit;
	}
	if(db_manager.UserRemove(test_usr,test_usr2)!=0){
		code=17;
		goto exit;
	}
	if(db_manager.UserRemove(test_usr,test_usr)!=0){
		code=18;
		goto exit;
	}
	if(db_manager.Disconnect(test_usr)!=0){
		code=19;
		goto exit;
	}

	ret=true;
exit:
	if(!ret){
		printf("DB_Manager users test error %d\n",code);
	}
	else{
		printf("DB_Manager users test success\n");
	}
	return ret;
}

static bool TestDBManagerData(DB_Manager &db_manager){
	bool ret=false;
	int code=0;
	std::string root(ROOT_DB_USER);
	std::string key1("key 1 1");
	std::string key2("key 2 2");
	std::string val1("value 1 1");
	std::string val2("value 2 2");
	std::string val2upd("updated 2 2");
	std::string result;

	//empty bd unconnectd
	if(db_manager.Get(root,key1,&result)==0){
		code=1;
		goto exit;
	}
	if(db_manager.Set(root,key1,val1)==0){
		code=2;
		goto exit;
	}

	//connected filling
	if(db_manager.Connect(root)!=0){
		code=3;
		goto exit;
	}
	if(db_manager.Set(root,key1,val1)!=0){
		code=4;
		goto exit;
	}
	if(db_manager.Get(root,key1,&result)!=0 && result!=val1){
		code=5;
		goto exit;
	}
	if(db_manager.Disconnect(root)!=0){
		code=6;
		goto exit;
	}

	//filled unconnected
	if(db_manager.Get(root,key1,&result)==0){
		code=7;
		goto exit;
	}
	if(db_manager.Set(root,key1,val1)==0){
		code=8;
		goto exit;
	}

	//removing
	if(db_manager.Connect(root)!=0){
		code=9;
		goto exit;
	}
	if(db_manager.Get(root,key1,&result)!=0 && result!=val1){
		code=10;
		goto exit;
	}
	if(db_manager.Remove(root,key1)!=0){
		code=11;
		goto exit;
	}
	if(db_manager.Get(root,key1,&result)==0){
		code=12;
		goto exit;
	}

	//rewriting
	if(db_manager.Get(root,key2,&result)==0){
		code=13;
		goto exit;
	}
	if(db_manager.Set(root,key2,val2)!=0){
		code=14;
		goto exit;
	}
	if(db_manager.Get(root,key2,&result)!=0 && result!=val2){
		code=15;
		goto exit;
	}
	if(db_manager.Set(root,key2,val2upd)!=0){
		code=16;
		goto exit;
	}
	if(db_manager.Get(root,key2,&result)!=0 && result!=val2upd){
		code=17;
		goto exit;
	}
	if(db_manager.Remove(root,key2)!=0){
		code=18;
		goto exit;
	}
	if(db_manager.Get(root,key2,&result)==0){
		code=19;
		goto exit;
	}
	if(db_manager.Disconnect(root)!=0){
		code=20;
		goto exit;
	}

	ret=true;
exit:
	if(!ret){
		printf("DB_Manager data test error %d\n",code);
	}
	else{
		printf("DB_Manager data test success\n");
	}
	return ret;
}

bool TestProtocolParser(){
	bool ret=false;
	int code=0;
	uint32_t pid=0x00ffffff;
	std::string root(ROOT_DB_USER);
	std::string key1("key 1 1");
	std::string val1("value 1 1");
	std::string result;
	std::list<std::string> list;
	std::list<std::string> list_readback;
	list.push_back(root);
	list.push_back(key1);
	list.push_back(val1);
	std::list<std::string>::iterator it1;
	std::list<std::string>::iterator it2;

	result.clear();
	if(UselessProtocolParser::PrepareMsgData(list,result)!=0){
		code=1;
		goto exit;
	}
	if(UselessProtocolParser::InjectPid(result,pid)!=0){
		code=2;
		goto exit;
	}
	if(UselessProtocolParser::WithdrawPid(result)!=pid){
		code=3;
		goto exit;
	}
	if(UselessProtocolParser::ParseMsg(result,list_readback)!=0){
		code=4;
		goto exit;
	}
	for(it1=list.begin(), it2=list_readback.begin();it1!=list.end() && it2!=list_readback.end();it1++, it2++){
		if(*it1!=*it2){
			code=5;
			goto exit;
		}
	}

	ret=true;
exit:
	if(!ret){
		printf("Protocol parser test error %d\n",code);
	}
	else{
		printf("Protocol parser test success\n");
	}
	return ret;
}

