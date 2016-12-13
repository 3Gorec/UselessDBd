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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
   #include <errno.h>

int main(void) {
	int ret=0;

	ret=daemon(0,1);
	if(ret==0){
		umask(0);
		printf("UselessDB daemon started\n");
	}
	else{
		printf("UselessDB daemon starting error %d\n",errno);
		return EXIT_FAILURE;
	}

	UselessDBDaemon daemon;
	ret=daemon.Init();
	if(ret==0){
		daemon.Run();
	}
	else{
		printf("UselessDB daemon init error\n");
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

