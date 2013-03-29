//
//  main.c
//
//  Created by Vitor Vezani on 23/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "globais.h"
#include <sys/time.h>		/* for gettimeofday() */
#include <stdio.h>		    /* for printf() */
#include <unistd.h>		    /* for fork() */
#include <sys/types.h>		/* for wait(), msgget(), msgctl() */
#include <sys/wait.h>		/* for wait() */
#include <sys/ipc.h>		/* for msgget(), msgctl() */
#include <sys/msg.h>		/* for msgget(), msgctl() */
#include <errno.h>          /* errno and error codes */
#include <signal.h>         /* for kill(), sigsuspend(), others */
#include <sys/shm.h>        /* for shmget(), shmat(), shmctl() */
#include <sys/sem.h>        /* for semget(), semop(), semctl() */
#include <stdlib.h>			/* for exit() */
#include <string.h>
#include <pthread.h>     	/* para poder manipular threads */

#define SHM_ID 1324

int main(int argc, char const *argv[]){

	int num_no; 
	char nome_arq[20];
	key_t key = SHM_ID;
	int shm_id;
	char *tmp_addr;

	//Testa Parametros
	if (argc != 3){
        printf("Use: %s 'nome_arq_config' 'numero_nó'\n", argv[0]);
        exit(1);
    }

	//Copia para as Variaveis
    strcpy(nome_arq,argv[1]);
	num_no = atoi(argv[2]);

	printf("nome do arquivo: '%s'\n num do nó: '%d'\n",nome_arq,num_no);

	//Cria a memoria compartilhada
	if((shm_id = shmget(key, sizeof(&shm_ren), IPC_CREAT | 0666)) == -1 ) {
		fprintf(stderr,"Impossivel criar a memoria compartilhada!\n");
		exit(1);
	}

	//Associa ao segmento de memoria compartilhada
	if((tmp_addr = (char *)shmat(shm_id, NULL, 0)) == (char *)-1 ) {
		fprintf(stderr,"Impossivel associar ao segmento de memoria compartilhada!\n");
		exit(1);
	}

	//Estabelecimento do ponteiro
	shm_ren = (shm_rede_enlace *)tmp_addr;

	//Inicializar Mutex
	pthread_mutex_init(&exc_aces, NULL);

	//Iniciar as Camadas
	iniciarEnlace(nome_arq,num_no);
	iniciarTesteEnlace(nome_arq,num_no);

	sleep(60);

	return 0;
}