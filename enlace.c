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
};

struct data_enlace{
	int type;
	char *data;
	int ecc;
};

void colocarArquivoStruct(FILE * fp, int lendo,struct ligacoes * ligacao);
void retirarEspaco(char * string);
void montarPacoteEnlace(struct data_enlace *datagram);
void *enviarPacotes(void *param);
void *receberPacotes(void *param);
int verificarECC(struct data_enlace *datagram);

void *iniciarEnlace(){

	int te, tr;
 	int result;
 	int lendo = 0;
 	int i,j;

 	pthread_t threadEnviarPacote, threadReceberPacote;

 	struct ligacoes ligacao;

	for (i = 0; i < 18 ; ++i)
		for (j = 0; j < 3; ++j)
			ligacao.enlaces[i][j] = 0;

		FILE * fp;
		fp = fopen(file_info.nome_arq, "r");

	    if(!fp){
	        perror("Fopen()");
	        exit(1);
	    }
	    
		colocarArquivoStruct(fp,lendo, &ligacao);

		printf("\nEnlace.c = > Tamanho do buffer '%d'\n", shm_ren_env.tam_buffer);

		te = pthread_create(&threadEnviarPacote, NULL, enviarPacotes,(void *)&ligacao);

		if (te){
  			printf("ERRO: impossivel criar a thread : Enviar Pacote\n");
  			exit(-1);
		}

		tr = pthread_create(&threadReceberPacote, NULL, receberPacotes, (void *)&ligacao);
		
		if (tr){
  			printf("ERRO: impossivel criar a thread : Receber Pacote\n");
  			exit(-1);
		}

	pthread_join(threadEnviarPacote, NULL);
	pthread_join(threadReceberPacote, NULL);
}

void *enviarPacotes(void *param){

	struct ligacoes *ligacaoo = (struct ligacoes *)param;

	struct ligacoes ligacao = *ligacaoo;

	struct data_enlace datagram_enlace_env;

	int atoi_result;
	struct sockaddr_in node;
	int i,j,s,flag;

	printf("\n");

	while(1){
		
		pthread_mutex_lock(&exc_aces);

		if(shm_ren_env.env_no != -1 && shm_ren_env.tam_buffer != 0){

			flag = 0;

		    fflush(stdin);
			printf("\nEnlace.c = > Tamanho do Pacote : '%d' Bytes\n", shm_ren_env.tam_buffer);

			for (i = 0; i < 18; ++i)
			{

				if((ligacao.enlaces[i][0] == file_info.num_no) && (shm_ren_env.env_no == ligacao.enlaces[i][1]))
				{

					if(shm_ren_env.tam_buffer > ligacao.enlaces[i][2]){
						printf("Enlace.c = > Erro de MTU\n");
						shm_ren_env.erro = ligacao.enlaces[i][2];
						flag = 2;
						break;
					}
					else
					{
						for(i = 0; i < 6; ++i)
						{
							atoi_result = atoi(ligacao.nos[i][0]);

							if (atoi_result == shm_ren_env.env_no)
							{
								if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
								perror("socket()");
								exit(1);
								}

								printf("Enlace.c = > Nó: '%d', IP: '%s' , Porta: '%d'\n",shm_ren_env.env_no,ligacao.nos[i][1],atoi(ligacao.nos[i][2]));

								node.sin_family = AF_INET; // Tipo do endereço         
							    node.sin_port = htons(atoi(ligacao.nos[i][2])); // Porta do servidor        
							    node.sin_addr.s_addr = inet_addr(ligacao.nos[i][1]); // Endereço IP do servidor  
							
							    printf("Enlace.c = > node Configurado\n");

								montarPacoteEnlace(&datagram_enlace_env);

								printf("Enlace.c = > Pacote Montado!\n");

								if (sendto(s, &datagram_enlace_env, sizeof(datagram_enlace_env), 0, (struct sockaddr *) &node, sizeof (node)) < 0) {
									perror("sendto()");
									exit(2);
								}

								#ifdef DEBBUG
								printf("Enlace.c = > Dados enviados!\n");
								#endif

								flag = 1;
								break;
							}
						}
					}
				}
			}

			if (flag == 0){
				shm_ren_env.erro = -1;
			}		
			else if(flag == 1){
				shm_ren_env.erro = 0;
			}

			printf("Enlace.c = > shm_ren_env.erro : '%d'\n\n",shm_ren_env.erro );

		    pthread_mutex_unlock(&exc_aces);
		}else
			pthread_mutex_unlock(&exc_aces);
	}
}

void *receberPacotes(void *param){

	struct ligacoes *ligacaoo = (struct ligacoes *)param;

	struct ligacoes ligacao = *ligacaoo;

	int i,atoi_result,ecc_result, s, client_address_size;
	struct sockaddr_in client, server;
	struct data_enlace datagram_enlace_rcv;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }

	for(i = 0; i < 6; ++i){

		atoi_result = atoi(ligacao.nos[i][0]);

		if(atoi_result == file_info.num_no){

			server.sin_family = AF_INET; /* Tipo do endereço             */
			server.sin_port = htons(5001); /* Escolhe uma porta disponível */
			server.sin_addr.s_addr = INADDR_ANY; /* Endereço IP do servidor */

			//IP's Variaveis de acordo com a tabela >erro no bind()<

    		//server.sin_port = htons(atoi(ligacao.nos[i][2])); /* Escolhe uma porta disponível */
    		//server.sin_addr.s_addr = inet_addr(ligacao.nos[i][1]); /* Endereço IP do servidor */
		}
	}
	
    if (bind(s, (struct sockaddr *) &server, sizeof (server)) < 0) {
        perror("bind()");
        exit(1);
    } 

    //printf("Enlace.c (server) => Escutando IP: '%s' Porta: '%d'\n",inet_ntoa(server.sin_addr),ntohs(server.sin_port));

	while(TRUE){

	    client_address_size = sizeof (client);

	    if (recvfrom(s, &datagram_enlace_rcv, sizeof (datagram_enlace_rcv), 0, (struct sockaddr *) &client,&client_address_size) < 0) {
	        perror("recvfrom()");
	        exit(1);
	    }

	    //printf("Enlace.c (server) => Recebida a mensagem do endereço IP %s da porta %d\n\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));
	    printf("Enlace.c (server) => Type: '%d', Data: '%s', ECC: '%d'\n",datagram_enlace_rcv.type,datagram_enlace_rcv.data,datagram_enlace_rcv.ecc);

	    ecc_result = verificarECC(&datagram_enlace_rcv);
	    if (ecc_result)
	    {
	    	printf("Datagrama sem erro\n");
	    }else
	    	printf("Datagrama corrompido\n");
   	}
}

void montarPacoteEnlace(struct data_enlace *datagram){

	int sum = 0;
	int i;

	datagram->type = shm_ren_env.type;

	if ((datagram->data = (char *) malloc(sizeof(char) * shm_ren_env.tam_buffer)) == NULL) {
        printf("unable to allocate memory \n");
        exit (4); 
    }

	memcpy(datagram->data, shm_ren_env.buffer, shm_ren_env.tam_buffer);

	for (i = 0; i < sizeof(datagram->data); ++i)
	{
		sum += datagram->data[i];
	}
	datagram->ecc = sum;

	printf("Enlace.c = > Type : '%d'-Data '%s'-ECC : '%d'\n",datagram->type,datagram->data,datagram->ecc );
}

int verificarECC(struct data_enlace *datagram){

	int sum = 0;
	int i;

	for (i = 0; i < sizeof(datagram->data); ++i)
	{
		sum += datagram->data[i];
	}

	printf("Enlace.c (server) = >ECC:'%d', Sum: '%d'\n",datagram->ecc,sum);

	if (datagram->ecc == sum)
		return 1;
	else
		return 0;

}

void colocarArquivoStruct(FILE * fp, int lendo, struct ligacoes *ligacao){

	size_t len= 100;
	char *linha= malloc(len);
	char *pch;
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
	   		deblank(pch);

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