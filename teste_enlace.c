//
//  teste_enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "globais.h"
#include <sys/time.h>		/* for gettimeofday() */
#include <stdio.h>			/* for printf() */
#include <unistd.h>			/* for fork() */
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
#include <pthread.h>		/* para poder manipular threads */

#define TRUE 	1
#define FALSE	0

void *enviarDados();
void *receberDados();

void iniciarTesteEnlace(){

	int te,tr;
	pthread_t threadEnviarDados,threadReceberDados;

	te = pthread_create(&threadEnviarDados, NULL, enviarDados,NULL);
	pthread_detach(threadEnviarDados);

	if (te){
  		printf("ERRO: impossivel criar a thread : Enviar Dados\n");
  		exit(-1);
	}

	tr = pthread_create(&threadReceberDados, NULL, receberDados, NULL);
	pthread_detach(threadReceberDados);

	if(tr){
  		printf("ERRO: impossivel criar a thread : Receber Dados\n");
  		exit(-1);
	}

	//pthread_join(threadEnviarDados, NULL);
	//pthread_join(threadReceberDados, NULL);

}

void *enviarDados(){
	
	char charopt[128];	

	while(1){

		pthread_mutex_lock(&exc_aces);

		fpurge(stdin);
    	fflush(stdin);

        printf ("Teste_enlace.c = > Digite o Conteudo de data: ");
		fgets(charopt , 127 , stdin);
		charopt[strlen(charopt)-1]='\0';

        strcpy(shm_ren_env.buffer,charopt);

        shm_ren_env.type = 2;
		shm_ren_env.tam_buffer = strlen(shm_ren_env.buffer);
		shm_ren_env.env_no = 2;
		printf("Teste_enlace.c = > Type: '%d', Num nó: '%d', Data: '%s', Tamanho : '%d'\n",shm_ren_env.type,shm_ren_env.env_no,shm_ren_env.buffer,shm_ren_env.tam_buffer);

	    pthread_mutex_unlock(&exc_aces);

	   	pthread_mutex_lock(&exc_aces);

	   	if (shm_ren_env.erro == 0)
	   	{
	   		printf("Teste_enlace.c = > Dados Enviados\n");
	   	}else if (shm_ren_env.erro == -1)
	   	{
	   		printf("Teste_enlace.c = > Não achou nó\n");
	   	}else if (shm_ren_env.erro > 0)
	   	{
	   		printf("Teste_enlace.c = > MTU excedido dividir o pacote no MAX em '%d' bytes \n",shm_ren_env.erro);
	   	}else
	   		printf("Teste_enlace.c = > Erro desconhecido\n");

	   	shm_ren_env.tam_buffer = 0;
		shm_ren_env.env_no = 0;
		strcpy(shm_ren_env.buffer,"");
		shm_ren_env.erro = 0;

	   	pthread_mutex_unlock(&exc_aces);
	

	}

}

void *receberDados(){
}