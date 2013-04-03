//
//  teste_enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/teste_enlace.h"

void *iniciarTesteEnlace(){

	int te,tr;
	pthread_t threadEnviarDatagramas,threadReceberDatagramas;

	te = pthread_create(&threadEnviarDatagramas, NULL, enviarDatagramas,NULL);

	if (te){
  		printf("ERRO: impossivel criar a thread : enviarDatagramas\n");
  		exit(-1);
	}

	tr = pthread_create(&threadReceberDatagramas, NULL, receberDatagramas, NULL);

	if(tr){
  		printf("ERRO: impossivel criar a thread : receberDatagramas\n");
  		exit(-1);
	}

	pthread_join(threadEnviarDatagramas, NULL);
	pthread_join(threadReceberDatagramas, NULL);
}

void *enviarDatagramas(){
	
	char charopt[128];	

	while(1){

		pthread_mutex_lock(&exc_aces);

		usleep(300);

		fpurge(stdin);
    	fflush(stdin);

        printf ("Teste_enlace.c = > Digite o Conteudo de data: ");
		fgets(charopt , 127 , stdin);
		charopt[strlen(charopt)-1]='\0';

        strcpy(shm_env.buffer,charopt);

        shm_env.type = 2;
		shm_env.tam_buffer = strlen(shm_env.buffer);
		shm_env.env_no = 2;

	    pthread_mutex_unlock(&exc_aces);

	   	pthread_mutex_lock(&exc_aces);

	   	if (shm_env.tam_buffer != 0)
	   	{
	   		printf("Teste_enlace.c = > Type: '%d', Num nó: '%d', Data: '%s', Tamanho : '%d'\n",shm_env.type,shm_env.env_no,shm_env.buffer,shm_env.tam_buffer);

	   		if (shm_env.erro == 0)
		   	{
		   		printf("Teste_enlace.c = > Dados Enviados\n");
		   	}else if (shm_env.erro == -1)
		   	{
		   		printf("Teste_enlace.c = > Não achou nó\n");
		   	}else if (shm_env.erro > 0)
		   	{
		   		printf("Teste_enlace.c = > MTU excedido dividir o pacote no MAX em '%d' bytes \n",shm_env.erro);
		   	}else
		   		printf("Teste_enlace.c = > Erro desconhecido\n");
	   	
		   	shm_env.tam_buffer = 0;
			shm_env.env_no = 0;
			strcpy(shm_env.buffer,"");
			shm_env.erro = 0;
	   	}

	   	pthread_mutex_unlock(&exc_aces);
	}

}

void *receberDatagramas(){

	while(TRUE){

		pthread_mutex_lock(&exc_aces2);

		if (shm_rcv.erro == 0)
		{
			printf("Teste_enlace.c = > Type: '%d', Tam_buffer: '%d'Bytes, Env_no: '%d',Buffer: '%s', Erro: '%d' \n",shm_rcv.type,shm_rcv.tam_buffer,shm_rcv.env_no,
				shm_rcv.buffer,shm_rcv.erro );

			shm_rcv.erro = -1;

			pthread_mutex_unlock(&exc_aces2);
		}else
			pthread_mutex_unlock(&exc_aces2);
	}
}