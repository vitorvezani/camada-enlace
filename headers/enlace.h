//
//  enlace.h
//
//  Created by Vitor Vezani on 26/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>		    /* for printf() */
#include <stdlib.h>			/* for exit() */
#include <string.h>
#include <pthread.h>     	/* para poder manipular threads */
#include <sys/socket.h>
#include <netinet/in.h>

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

struct frame{
	int tam_buffer_frame;
	struct datagrama data;
	char ecc;
};

#define TRUE 			1
#define FALSE 			0

#define NOS 			1
#define ENLACES 		2

extern struct datagrama shm_ren_env,shm_ren_rcv;
extern struct file file_info;
extern pthread_mutex_t exc_aces,exc_aces2;

void colocarArquivoStruct(FILE * fp,struct ligacoes * ligacao);
void retirarEspaco(char * string);
void montarFrame(struct frame *datagram);
void montarDatagrama(struct frame datagram);
void *enviarPacotes(void *param);
void *receberPacotes(void *param);
int checkSum(struct frame datagram);