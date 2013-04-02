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

struct ligacoes{
    char nos[6][3][25];
    int enlaces[18][3];
};

struct data_enlace{
	int tam_dados;
	struct shm_rede_enlace data;
	char *ecc;
};

#define DEBBUG

#define TRUE 			1
#define FALSE 			0

#define NOS 			1
#define ENLACES 		2

extern struct shm_rede_enlace shm_ren_env,shm_ren_rcv;
extern struct shd_file_info file_info;
extern pthread_mutex_t exc_aces,exc_aces2;

void colocarArquivoStruct(FILE * fp,struct ligacoes * ligacao);
void retirarEspaco(char * string);
void montarPacoteEnlace(struct data_enlace *datagram);
void montarPacoteRede(struct data_enlace datagram);
void *enviarPacotes(void *param);
void *receberPacotes(void *param);
int verificarECC(struct data_enlace *datagram);