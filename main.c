//
//  main.c
//
//  Created by Vitor Vezani on 23/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "globais.h"

void *iniciarEnlace();
void *iniciarTesteEnlace();

int main(int argc, char const *argv[]){

	int te,tite;
	pthread_t threadIniciaEnlace,threadIniciaTesteEnlace;

	//Testa Parametros
	if (argc != 3){
        printf("Use: %s 'nome_arq_config' 'numero_nó'\n", argv[0]);
        exit(1);
    }

	//Copia para as Variaveis
    strcpy(file_info.nome_arq,argv[1]);
	file_info.num_no = atoi(argv[2]);

	printf("nome do arquivo: '%s'\n num do nó: '%d'\n",file_info.nome_arq,file_info.num_no);

	//inicializacao do buffer Rede->Enlace
	shm_ren_env.env_no = -1;
	strcpy(shm_ren_env.buffer,"");
	shm_ren_env.tam_buffer = strlen(shm_ren_env.buffer);

	//Inicializar Mutex
	pthread_mutex_init(&exc_aces, NULL);

	//Iniciar as Camadas
	te = pthread_create(&threadIniciaEnlace, NULL, iniciarEnlace, NULL);

	if (te){
			printf("ERRO: impossivel criar a thread : iniciarEnlace\n");
			exit(-1);
	}

	usleep(400);

	tite = pthread_create(&threadIniciaTesteEnlace, NULL, iniciarTesteEnlace, NULL);

	if (tite){
			printf("ERRO: impossivel criar a thread : iniciarTesteEnlace\n");
			exit(-1);
	}

	pthread_join(threadIniciaEnlace, NULL);
	pthread_join(threadIniciaTesteEnlace, NULL);

  	pthread_mutex_destroy(&exc_aces);

	return 0;
}