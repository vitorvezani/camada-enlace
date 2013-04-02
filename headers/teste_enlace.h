//
//  teste_enlace.h
//
//  Created by Vitor Vezani on 26/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>		    /* for printf() */
#include <stdlib.h>			/* for exit() */
#include <string.h>
#include <pthread.h>     	/* para poder manipular threads */

#define TRUE 	1
#define FALSE	0

struct shm_rede_enlace{
	int type;
	int tam_buffer;
	int env_no;
	char buffer[100];
	int erro;
};

struct shd_file_info{
	char nome_arq[20];
	int num_no;
};

extern struct shm_rede_enlace shm_ren_env,shm_ren_rcv;
extern struct shd_file_info file_info;
extern pthread_mutex_t exc_aces,exc_aces2;

void *enviarDados();
void *receberDados();