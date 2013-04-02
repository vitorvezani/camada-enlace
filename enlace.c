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

 	pthread_t threadEnviarPacote, threadReceberPacote;

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

	int i,j,flag;

	printf("\n");

	while(1){

		struct frame frame_env;
		struct sockaddr_in node;
		int atoi_result = -1;
		int s;
		
		pthread_mutex_lock(&exc_aces);

		if(shm_ren_env.env_no != -1 && shm_ren_env.tam_buffer != 0){

			flag = 0;

		    fflush(stdin);

			for (i = 0; i < 18; ++i)
			{

				if((ligacao.enlaces[i][0] == file_info.num_no) && (shm_ren_env.env_no == ligacao.enlaces[i][1]))
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

							printf("Enlace.c = > Achou! Nó: '%d', IP: '%s' , Porta: '%d'\n",shm_ren_env.env_no,ligacao.nos[i][1],atoi(ligacao.nos[i][2]));

							node.sin_family = AF_INET; // Tipo do endereço         
						    node.sin_port = htons(atoi(ligacao.nos[i][2])); // Porta do servidor        
						    node.sin_addr.s_addr = inet_addr(ligacao.nos[i][1]); // Endereço IP do servidor  
						
						    printf("Enlace.c = > Nó Configurado\n");

							montarFrame(&frame_env);

							if(frame_env.tam_buffer_frame > ligacao.enlaces[i][2]){
								printf("Enlace.c = > Erro de MTU\n");
								shm_ren_env.erro = ligacao.enlaces[i][2];
								flag = 2;
								break;
							}

							printf("Enlace.c = > Frame Montado! (tam_buffer_frame: '%d', ecc: '%d', tam_data: '%lu', tam_frame: '%lu'\n",frame_env.tam_buffer_frame,
								frame_env.ecc,sizeof(frame_env.data),sizeof(frame_env));

							frame_env.ecc = checkSum(frame_env);

							printf("Enlace.c = > ECC Calculado! (tam_buffer_frame: '%d', ecc: '%d', tam_data: '%lu', tam_frame: '%lu'\n",frame_env.tam_buffer_frame,
								frame_env.ecc,sizeof(frame_env.data),sizeof(frame_env));


							if (sendto(s, &frame_env, sizeof(frame_env), 0, (struct sockaddr *) &node, sizeof (node)) < 0) {
								perror("sendto()");
								exit(2);
							}

							#ifdef DEBBUG
							printf("Enlace.c = > Dados enviados!\n");
							#endif

							flag = 1;
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

	int i,atoi_result, s, client_address_size;
	struct sockaddr_in client, server;

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

    //printf("Enlace.c (server) = > Escutando IP: '%s' Porta: '%d'\n",inet_ntoa(server.sin_addr),ntohs(server.sin_port));

	while(TRUE){

		struct frame frame_rcv;
		int ecc, sum = 0;

	    client_address_size = sizeof (client);

	    if (recvfrom(s, &frame_rcv, sizeof (frame_rcv), 0, (struct sockaddr *) &client,&client_address_size) < 0) {
	        perror("recvfrom()");
	        exit(1);
	    }

	    //printf("Enlace.c (server) = > Recebida a mensagem do endereço IP %s da porta %d\n\n",inet_ntoa(client.sin_addr), ntohs(client.sin_port));

	   	printf("Enlace.c = > Frame Recebido! (tam_buffer_frame: '%d', ecc: '%d', tam_data: '%lu', tam_frame: '%lu'\n",frame_rcv.tam_buffer_frame,
			frame_rcv.ecc,sizeof(frame_rcv.data),sizeof(frame_rcv));

	    ecc = frame_rcv.ecc;

	    frame_rcv.ecc =0;

	    sum = checkSum(frame_rcv);

	    if (ecc == sum)
	    {
	    	printf("Enlace.c (server) = > Datagrama sem erro\n");
	    	montarDatagrama(frame_rcv);
	    }else
	    	printf("Enlace.c (server) = > Datagrama corrompido - Pacote Descartado\n");

	printf("Enlace.c (server) = > ECC:'%d', Sum: '%d'\n",ecc,sum);
   	}
}

void montarDatagrama(struct frame datagram){

	pthread_mutex_lock(&exc_aces2);

		memcpy(&shm_ren_rcv, &datagram.data, sizeof(datagram.data));

		shm_ren_rcv.tam_buffer = datagram.tam_buffer_frame;

		shm_ren_rcv.env_no = -1;
		shm_ren_env.erro = 0;

	pthread_mutex_unlock(&exc_aces2);

}

void montarFrame(struct frame *datagram){

	datagram->ecc = 0;

	datagram->tam_buffer_frame = shm_ren_env.tam_buffer;

	memcpy(&datagram->data, &shm_ren_env, sizeof(shm_ren_env));
}

int checkSum(struct frame datagram){

	int sum = 0;
	int aux = 0;
	int i;
	void *ptr = &datagram;

	for (i = 0; i < sizeof(datagram); i += sizeof(int))
	{
		memcpy(&aux, ptr, sizeof(int));
		ptr += sizeof(int);
		sum += aux ;
	}
	return sum;
}