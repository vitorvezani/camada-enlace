//
//  enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/arquivo.h"
#include "headers/enlace.h"

void *iniciarEnlace(){

	int te, tr;
 	int result;
 	int i,j;

 	pthread_t threadEnviarFrames, threadReceberFrames;

 	struct ligacoes ligacao;

	for (i = 0; i < 18 ; ++i)
		for (j = 0; j < 3; ++j)
			ligacao.enlaces[i][j] = 0;

		FILE * fp;
		fp = fopen(file_info.file_name, "r");

	    if(!fp){
	        perror("Fopen()");
	        exit(1);
	    }
	    
		colocarArquivoStruct(fp, &ligacao);

		te = pthread_create(&threadEnviarFrames, NULL, enviarFrames,(void *)&ligacao);

		if (te){
  			printf("ERRO: impossivel criar a thread : Enviar Pacote\n");
  			exit(-1);
		}

		tr = pthread_create(&threadReceberFrames, NULL, receberFrames, (void *)&ligacao);
		
		if (tr){
  			printf("ERRO: impossivel criar a thread : Receber Pacote\n");
  			exit(-1);
		}

	pthread_join(threadEnviarFrames, NULL);
	pthread_join(threadReceberFrames, NULL);
}

void *enviarFrames(void *param){

	struct ligacoes *ligacaoo = (struct ligacoes *)param;

	struct ligacoes ligacao = *ligacaoo;

	int i,j,flag;

	#ifdef DEBBUG_ENLACE
	printf("\n");
	#endif

	while(1){

		struct frame frame_env;
		struct sockaddr_in to;
		int atoi_result = -1;
		int s,mtu;
		
		pthread_mutex_lock(&exc_aces);

		if(shm_env.env_no != -1 && shm_env.tam_buffer != 0){

			flag = 0;

		    fflush(stdin);

			for (i = 0; i < 18; ++i)
			{
				//Verificar se existe ligacao entre seu nó e o nó destino
				if((ligacao.enlaces[i][0] == file_info.num_no) && (shm_env.env_no == ligacao.enlaces[i][1]))
				{
					#ifdef DEBBUG_ENLACE
					printf("Enlace.c = > Existe Ligacao nos [Enlaces]\n");
					#endif

					mtu = ligacao.enlaces[i][2];

					for(i = 0; i < 6; ++i)
					{
						atoi_result = atoi(ligacao.nos[i][0]);

						//Verificar o IP e Porta do nó destino
						if (atoi_result == shm_env.env_no)
						{
							if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
							perror("socket()");
							exit(1);
							}

							#ifdef DEBBUG_ENLACE
							printf("Enlace.c = > Existe -> [Nó]: '%d',possui IP: '%s' , Porta: '%d'\n",shm_env.env_no,ligacao.nos[i][1],atoi(ligacao.nos[i][2]));
							#endif

							to.sin_family = AF_INET; // Tipo do endereço         
						    to.sin_port = htons(atoi(ligacao.nos[i][2])); // Porta do servidor        
						    to.sin_addr.s_addr = inet_addr(ligacao.nos[i][1]); // Endereço IP do servidor  
							
							#ifdef DEBBUG_ENLACE
						    printf("Enlace.c = > Nó Configurado\n");
						    #endif

							montarFrame(&frame_env);

							#ifdef DEBBUG_ENLACE
							printf("Enlace.c = > Frame Montado! tam_buffer_frame: '%d', tam_data: '%lu', tam_frame: '%lu'\n",frame_env.tam_buffer_frame,
								sizeof(frame_env.data),sizeof(frame_env));
							#endif

							if(sizeof(frame_env) > mtu){
								printf("Enlace.c = > Erro de MTU\n");
								shm_env.erro = mtu;
								flag = 2;
								break;
							}

							#ifdef DEBBUG_ENLACE
							printf("Enlace.c = > sizeof(Frame): '%lu', MTU: '%d'\n",sizeof(frame_env),ligacao.enlaces[i][2]);
							#endif	

							frame_env.ecc = checkSum(shm_env);

							#ifdef DEBBUG_ENLACE
							printf("Enlace.c = > ECC Calculado! ecc: '%d'\n",frame_env.ecc);
							#endif	

							set_garbler(0,0,0);

							if (sendto_garbled(s, &frame_env, sizeof(frame_env), 0,(struct sockaddr *) &to,sizeof (to)) < 0){
								perror("sendto()");
								printf("Enlace.c = > Dados não enviados!\n");
							}else{
								printf("Enlace.c = > Dados enviados!\n");
								flag = 1;
							}

						}
					}
				}
			}

			if (flag == 0){
				shm_env.erro = -1;
			}		
			else if(flag == 1){
				shm_env.erro = 0;
			}

			#ifdef DEBBUG_ENLACE
			printf("Enlace.c = > Setado variavel de erro shm_env: '%d'\n\n",shm_env.erro );
			#endif

		    pthread_mutex_unlock(&exc_aces);
		}else
			pthread_mutex_unlock(&exc_aces);
	}
}

void *receberFrames(void *param){

	struct ligacoes *ligacaoo = (struct ligacoes *)param;

	struct ligacoes ligacao = *ligacaoo;

	int i,atoi_result, s, from_address_size;
	struct sockaddr_in from, server;

	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket()");
        exit(1);
    }

	for(i = 0; i < 6; ++i){

		atoi_result = atoi(ligacao.nos[i][0]);

		if(atoi_result == file_info.num_no){

			#ifdef DEBBUG_ENLACE
			//printf("Enlace.c (server) = > Escutando IP: '%s' Porta: '%s'\n",ligacao.nos[i][1],ligacao.nos[i][2]);
			#endif

			server.sin_family = AF_INET; /* Tipo do endereço             */
			server.sin_port = htons(atoi(ligacao.nos[i][2])); /* Escolhe uma porta disponível */
    		server.sin_addr.s_addr = inet_addr(ligacao.nos[i][1]); /* Endereço IP do servidor */

			//server.sin_port = htons(5001); /* Escolhe uma porta disponível */
			//server.sin_addr.s_addr = INADDR_ANY; /* Endereço IP do servidor */
		}
	}
	
    if (bind(s, (struct sockaddr *) &server, sizeof (server)) < 0) {
        perror("bind()");
        exit(1);
    }

	while(TRUE){

		struct frame frame_rcv;
		int sum = 0;

	    from_address_size = sizeof (from);

	    if (recvfrom(s, &frame_rcv, sizeof (frame_rcv), 0, (struct sockaddr *) &from,&from_address_size) < 0) {
	        perror("recvfrom()");
	        exit(1);
	    }

	   	printf("\nEnlace.c (server)= > Frame Recebido! tam_buffer_frame: '%d', ecc: '%d', tam_datagrama: '%lu', tam_frame: '%lu'\n",frame_rcv.tam_buffer_frame,
			frame_rcv.ecc,sizeof(frame_rcv.data),sizeof(frame_rcv));

	   	montarDatagrama(frame_rcv);

	   	printf("Enlace.c (server)= > Datagrama Montado!\n");

	    sum = checkSum(shm_rcv);

	    printf("Enlace.c (server) = > ECC Recalculado -> frame_rcv.ECC:'%d', ECC recalculado: '%d'\n",frame_rcv.ecc,sum);

	    if (frame_rcv.ecc == sum)
	    	printf("Enlace.c (server) = > Datagrama sem erro\n");
	  	else{
	    	printf("Enlace.c (server) = > Datagrama corrompido - Pacote Descartado\n");
	    	shm_rcv.erro = -1;
	    }

	pthread_mutex_unlock(&exc_aces2);

   	}
}

void montarDatagrama(struct frame datagram){
			
	pthread_mutex_lock(&exc_aces2);

	memcpy(&shm_rcv, &datagram.data, sizeof(datagram.data));

}

void montarFrame(struct frame *datagram){

	datagram->ecc = 0;

	datagram->tam_buffer_frame = shm_env.tam_buffer;

	shm_env.env_no = -1;
	shm_env.erro = 0;

	memcpy(&datagram->data, &shm_env, sizeof(shm_env));

}

int checkSum(struct datagrama datagram){

	int sum = 0;
	int aux = 0;
	int i;
	void *ptr = &datagram;

	for (i = 0; i < sizeof(datagram); i += sizeof(int))
	{
		memcpy(&aux, ptr, sizeof(int));
		ptr += sizeof(int);
		sum += aux;
	}
	return sum;
}