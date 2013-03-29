//
//  teste_enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "globais.h"
#include <sys/time.h>		/* for gettimeofday() */
#include <stdio.h>		/* for printf() */
#include <unistd.h>		/* for fork() */
#include <sys/types.h>		/* for wait(), msgget(), msgctl() */
#include <sys/wait.h>		/* for wait() */
#include <sys/ipc.h>		/* for msgget(), msgctl() */
#include <sys/msg.h>		/* for msgget(), msgctl() */
#include <errno.h>              /* errno and error codes */
#include <signal.h>             /* for kill(), sigsuspend(), others */
#include <sys/shm.h>            /* for shmget(), shmat(), shmctl() */
#include <sys/sem.h>            /* for semget(), semop(), semctl() */
#include <stdlib.h>		/* for exit() */
#include <string.h>
#include <pthread.h>			/* para poder manipular threads */

#define SHM_ID 1321

#define TRUE 	1
#define FALSE	0

void *enviarDados();
void *ReceberDados();

void iniciarTesteEnlace(){

	int te,tr;
	pthread_t threadEnviarDados,threadReceberDados;

	te = pthread_create(&threadEnviarDados, NULL, enviarDados,NULL);
	pthread_detach(threadEnviarDados);

	if (te){
  		printf("ERRO: impossivel criar a thread : Enviar Dados\n");
  		exit(-1);
	}

	/*

	tr = pthread_create(&threadReceberDados, NULL, receberDados, NULL);
	pthread_detach(threadReceberDados);

	if(tr){
  		printf("ERRO: impossivel criar a thread : Receber Dados\n");
  		exit(-1);
	}
	*/

	pthread_join(threadEnviarDados, NULL);
	//pthread_join(threadReceberDados, NULL);
 
  	pthread_mutex_destroy(&exc_aces);

}

void *enviarDados(){

	while(TRUE){

		pthread_mutex_lock(&exc_aces);

        strcpy(shm_ren->buffer,"AAAAAAAAAAA");

		shm_ren->tam_buffer = strlen(shm_ren->buffer);
		printf("Tamanho : %d\n",shm_ren->tam_buffer);

	    pthread_mutex_unlock(&exc_aces);
	}

}

void *receberDados(){
}