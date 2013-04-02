//
//  main.h
//
//  Created by Vitor Vezani on 26/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>		    /* for printf() */
#include <stdlib.h>			/* for exit() */
#include <string.h>
#include <pthread.h>     	/* para poder manipular threads */

struct datagrama{
	int type;
	int tam_buffer;
	int env_no;
	char buffer[100];
	int erro;
};

struct file{
	char file_name[20];
	int num_no;
};

struct datagrama shm_ren_env,shm_ren_rcv;
struct file file_info;
pthread_mutex_t exc_aces,exc_aces2;

void *iniciarEnlace();
void *iniciarTesteEnlace();