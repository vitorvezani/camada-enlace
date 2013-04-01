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

int main(int argc, char const *argv[]){

	int num_no; 
	char nome_arq[20];

	//Testa Parametros
	if (argc != 3){
        printf("Use: %s 'nome_arq_config' 'numero_nó'\n", argv[0]);
        exit(1);
    }

	//Copia para as Variaveis
    strcpy(nome_arq,argv[1]);
	num_no = atoi(argv[2]);

	printf("nome do arquivo: '%s'\n num do nó: '%d'\n",nome_arq,num_no);

	//inicializacao do buffer Rede->Enlace
	shm_ren_env.env_no = -1;
	strcpy(shm_ren_env.buffer,"");
	shm_ren_env.tam_buffer = strlen(shm_ren_env.buffer);

	//Inicializar Mutex
	pthread_mutex_init(&exc_aces, NULL);

	//Iniciar as Camadas
	iniciarEnlace(nome_arq,num_no);
	iniciarTesteEnlace();

  	pthread_mutex_destroy(&exc_aces);

	sleep(60);

	return 0;
}