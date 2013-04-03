//
//  main.c
//
//  Created by Vitor Vezani on 23/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/main.h"

int main(int argc, char const *argv[]){
	
	int te,tite;
	pthread_t threadIniciaEnlace,threadIniciaTesteEnlace;

	//Testa Parametros
	if (argc != 3){
        printf("Use: %s 'file_name.ini' 'numero_nó'\n", argv[0]);
        exit(1);
    }

	//Copia para as Variaveis
    strcpy(file_info.file_name,argv[1]);
	file_info.num_no = atoi(argv[2]);

	printf("nome do arquivo: '%s'\n num do nó: '%d'\n",file_info.file_name,file_info.num_no);

	//inicializacao do buffer Rede->Enlace(Enviar)
	shm_env.erro = -9;
	shm_env.env_no = -1;
	strcpy(shm_env.buffer,"");
	shm_env.tam_buffer = strlen(shm_env.buffer);

	//inicializacao do buffer Rede->Enlace(Receber)
	shm_rcv.erro = -1;

	//Inicializar Mutex
	pthread_mutex_init(&exc_aces, NULL);
	pthread_mutex_init(&exc_aces2, NULL);

	//Inicia Camada de Enlace
	te = pthread_create(&threadIniciaEnlace, NULL, iniciarEnlace, NULL);

	if (te){
			printf("ERRO: impossivel criar a thread : iniciarEnlace\n");
			exit(-1);
	}

	usleep(800);

	//Inicia Camada de TesteEnlace
	tite = pthread_create(&threadIniciaTesteEnlace, NULL, iniciarTesteEnlace, NULL);

	if (tite){
			printf("ERRO: impossivel criar a thread : iniciarTesteEnlace\n");
			exit(-1);
	}

	//Espera as threads terminarem
	pthread_join(threadIniciaEnlace, NULL);
	pthread_join(threadIniciaTesteEnlace, NULL);

	//Destroi o Mutex
  	pthread_mutex_destroy(&exc_aces);
  	pthread_mutex_destroy(&exc_aces2);

	return 0;
}