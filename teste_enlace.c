//
//  teste_enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/teste_enlace.h"

void *iniciarTesteEnlace(){

	int te,tr;
	pthread_t threadEnviarDados,threadReceberDados;

	te = pthread_create(&threadEnviarDados, NULL, enviarDados,NULL);

	if (te){
  		printf("ERRO: impossivel criar a thread : Enviar Dados\n");
  		exit(-1);
	}

	tr = pthread_create(&threadReceberDados, NULL, receberDados, NULL);

	if(tr){
  		printf("ERRO: impossivel criar a thread : Receber Dados\n");
  		exit(-1);
	}
}

void *enviarDados(){
	
	char charopt[128];	

	while(1){

		pthread_mutex_lock(&exc_aces);

		usleep(300);

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
	/*TODO*/
}