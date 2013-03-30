//
//  enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "globais.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>		/* for gettimeofday() */
#include <stdio.h>			/* for printf() */
#include <unistd.h>			/* for fork() */
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

#define DEBBUG

#define TRUE 			1
#define FALSE 			0

#define NOS 			1
#define ENLACES 		2

struct ligacoes{
    char nos[6][3][25];
    int enlaces[18][3];
    int num_no;
};

struct data_enlace{
	int type;
	char *data;
	int ecc;
};

void colocarArquivoStruct(FILE * fp, int lendo,struct ligacoes * ligacao);
void retirarEspaco(char * string);
void montarPacoteEnlace(struct data_enlace *datagrama_enlace);
void *enviarPacote(void *param);
void *ReceberPacote();

void iniciarEnlace(char * nome_arq,int num_no){

	int te, tr;
 	int result;
 	int lendo = 0;
 	int i,j;

 	pthread_t threadEnviarPacote, threadReceberPacote;

 	struct ligacoes ligacao;

 	ligacao.num_no = num_no;

	for (i = 0; i < 18 ; ++i)
		for (j = 0; j < 3; ++j)
			ligacao.enlaces[i][j] = 0;

		FILE * fp;
		fp = fopen(nome_arq, "r");

	    if(!fp){
	        perror("Fopen()");
	        exit(1);
	    }
	    
		colocarArquivoStruct(fp,lendo, &ligacao);

		printf("tamanho do buffer %d\n", shm_ren.tam_buffer);

		te = pthread_create(&threadEnviarPacote, NULL, enviarPacote,(void *)&ligacao);
		pthread_detach(threadEnviarPacote);
		
		if (te){
  			printf("ERRO: impossivel criar a thread : Enviar Pacote\n");
  			exit(-1);
		}

	/*
		tr = pthread_create(&threadReceberPacote, NULL, receberPacote, NULL)
		pthread_detach(threadReceberPacote);
		
		if (te){
  			printf("ERRO: impossivel criar a thread : Receber Pacote\n");
  			exit(-1);
		}

	*/

	pthread_join(threadEnviarPacote, NULL);
	//pthread_join(threadReceberPacote, NULL);
 
  	//pthread_mutex_destroy(&exc_aces);
}

void *enviarPacote(void *param){

	struct ligacoes *ligacao = (struct ligacoes *)param;

	struct data_enlace datagrama_enlace;

	int i,j,s,flag;
	int atoi_result;
	struct sockaddr_in server;

	while(1){
		
		flag = 0;

		if (shm_ren.tam_buffer == 0)
			break;

		fflush(stdin);
		printf("estou preso\n");
		fflush(stdin);

	    pthread_mutex_lock(&exc_aces);

		fflush(stdin);
		printf("estou livre\n");

	    fflush(stdin);
		printf("\nTamanho do Pacote : %d Bytes\n", shm_ren.tam_buffer);

		for (i = 0; i < 18; ++i)
		{
			if(ligacao->enlaces[i][0] == ligacao->num_no && shm_ren.env_no == ligacao->enlaces[i][1])
			{

				if(shm_ren.tam_buffer > ligacao->enlaces[i][2]){
					printf("Erro de MTU\n");
					shm_ren.erro = ligacao->enlaces[i][2];
					flag = 2;
					break;
				}
				else
				{
					for(i = 0; i < 6; ++i)
					{
						atoi_result = atoi(ligacao->nos[i][0]);

						if (atoi_result == shm_ren.env_no)
						{
							if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
							perror("socket()");
							exit(1);
							}

							printf("ip:%s , porta: %d\n",ligacao->nos[i][1],atoi(ligacao->nos[i][2]));

							server.sin_family = AF_INET; /* Tipo do endereço         */
						    server.sin_port = htons(atoi(ligacao->nos[i][2])); /* Porta do servidor        */
						    server.sin_addr.s_addr = inet_addr(ligacao->nos[i][1]); /* Endereço IP do servidor  */
						
							montarPacoteEnlace(&datagrama_enlace);

							if (sendto(s, &datagrama_enlace, sizeof(datagrama_enlace), 0, (struct sockaddr *) &server, sizeof (server)) < 0) {
								perror("sendto()");
								exit(2);
							}

							#ifdef DEBBUG
							printf("\nDados enviados!\n\n");
							#endif

							flag = 1;
						}
					}
				}
			}
		}

		if (flag == 0)
			shm_ren.erro = -1;
		else if(flag == 1)
			shm_ren.erro = 0;

	    pthread_mutex_unlock(&exc_aces);
	}
}

void *receberPacotes(){

}

void montarPacoteEnlace(struct data_enlace *datagrama_enlace){

	int sum = 0;
	int i;

	memcpy(datagrama_enlace->data, shm_ren.buffer, shm_ren.tam_buffer);

	for (i = 0; i < shm_ren.tam_buffer; ++i)
	{
		sum += datagrama_enlace->data[i];
	}
	datagrama_enlace->ecc = sum;
}

void colocarArquivoStruct(FILE * fp, int lendo,struct ligacoes *ligacao){

	size_t len= 100; // valor arbitrário
	char *linha= malloc(len);
	char * pch;
	int j,i=0;
	int troca_i;

	while (getline(&linha, &len, fp) > 0)
	{
	j=0;
	troca_i = 0;

	pch = strtok (linha,">,:");

		if (strlen(linha) != 1){ //enter somente com o '\n'

		  while (pch != NULL)
	  {
	    retirarEspaco(pch);

	    if (strcmp(pch,"[Nos]") == 0)
	    {	
	    	#ifdef DEBBUG
	    	printf("\nTabela de nós\n");
	    	#endif
		    lendo = NOS;
		    i=0;
	    }

	    if (strcmp(pch,"[Enlaces]") == 0)
	    {
	    	#ifdef DEBBUG
	    	printf("\nTabela de enlaces\n");
	    	#endif
		    lendo = ENLACES;
		    i=0;
	    }

	    if (strcmp(pch,"[Nos]") != 0 && strcmp(pch,"[Enlaces]") != 0)
	    {
	    	if (lendo == NOS)
	    	{
				strcpy(ligacao->nos[i][j],pch);
				#ifdef DEBBUG
				printf("nos[%d][%d] '%s' | ",i,j,ligacao->nos[i][j]);
				#endif
				troca_i++;
	    	}else if (lendo = ENLACES)
	    	{
				ligacao->enlaces[i][j] = atoi(pch);
				#ifdef DEBBUG
				printf("enlace[%d][%d] '%d' | ",i,j,ligacao->enlaces[i][j]);
	    		#endif
	    		troca_i++;
	    	}
			if(troca_i == 3){
				i++;
				#ifdef DEBBUG
				printf("\n");
				#endif
			}
	    }
	    j++;
	    pch = strtok (NULL, ">,:");
	  }
	}
	}

	if(linha)
		free(linha);

	fclose(fp);
}