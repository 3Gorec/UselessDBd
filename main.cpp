//============================================================================
// Name        : UselessDBd.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C, Ansi-style
//============================================================================

#include "db_manager.h"
#include "uselessdb_daemon.h"
#include "tests.h"
#include <stdio.h>


int main(void) {
	int ret=0;

	UselessDBDaemon daemon;
	ret=daemon.Init();
	if(ret==0){
		daemon.Run();
	}
	else{
		printf("daemon init error\n");
	}

	return EXIT_SUCCESS;
}

