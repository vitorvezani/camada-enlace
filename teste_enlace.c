//
//  teste_enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_ID 1321

typedef struct{
	int tam_buffer;
	int env_no;
	char *buffer;
	int errno;
}shm_rede_enlace;

pthread_mutex_t produtor;
pthread_mutex_t consumidor;

shm_rede_enlace *shm_ren;

int main(int argc, char const *argv[])
{
	int num_no; 
	char nome_arq[20];

	key_t key = SHM_ID;
	int shm_id;

	char *tmp_addr;

	if (argc != 3) {
        printf("Use: %s 'nome_arq_config' 'numero_nó'\n", argv[0]);
        exit(1);
    }

    strcpy(nome_arq,argv[1]);
	num_no = atoi(argv[2]);

	printf("nome do arquivo: '%s'\n num do nó: '%d'\n",nome_arq,num_no);

	//Cria a memoria compartilhada
	if( (shm_id = shmget(key, sizeof(shm_rede_enlace), IPC_CREAT | 0666)) == -1 ) {
		fprintf(stderr,"Impossivel criar a memoria compartilhada!\n");
		exit(1);
	}

	//Associa ao segmento de memoria compartilhada
	if( (tmp_addr = (char *)shmat(shm_id, NULL, 0)) == (char *)-1 ) {
		fprintf(stderr,"Impossivel associar ao segmento de memoria compartilhada!\n");
		exit(1);
	}

	//Estabelecimento do ponteiro
	shm_ren = (shm_rede_enlace *)tmp_addr;

	//Inicializar Mutex
	pthread_mutex_init(&produtor, NULL);
  	pthread_mutex_init(&consumidor, NULL);

	iniciarEnlace(nome_arq,num_no);

	return 0;
}