//
//  enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define DEBBUG
#define NOS 			1
#define ENLACES 		2

void colocarArquivoStruct(FILE * fp, int lendo);
void retirarEspaco(char * string);

struct ligacoes{
    char nos[6][3][25];
    int enlaces[18][3];
    int num_no;
};

struct shm_rede_enlace{
	int tam_buffer;
	int env_no;
	char *buffer;
	char *buffer_return;
};

struct data_enlace{
	int type;
	char *data;
	int ecc;
};

/*
void gerarECC(shm_rede_enlace,data_enlace){

	int sum = 0;

	for (int i = 0; i < shm_rede_enlace.tam_buffer; ++i)
	{
		sum += data_enlace.data[i];
	}
}
*/

void iniciarEnlace(char * nome_arq,int num_no){

 	int result;
 	int lendo = 0;
 	int i,j;
 	pthread_t = thread_enviarPacote;
 	struct ligacoes ligacao;

 	ligacao.num_no = num_no;

	for (i = 0; i < 18 ; ++i)
	{
		for (j = 0; j < 3; ++j)
		{
			ligacao.enlaces[i][j] = 0;
		}
	}

		FILE * fp;
		fp = fopen(nome_arq, "r");

	    if(!fp){
	        perror("Fopen()");
	        exit(1);
	    }
	    
		colocarArquivoStruct(fp,lendo);
		fclose (fp);

		pthread_create(&thread_enviarPacote, NULL, enviarPacote,(void *)&ligacao)
		pthread_detach(thread_enviarPacote);

/*
		pthread_create(&thread_enviarPacote, NULL, enviarPacote, NULL)
		pthread_detach(thread_enviarPacote);
*/
}

void *enviarPacote(void *param){

	struct ligacoes *ligacao = (struct ligacoes *)param;

	int i,j,s;
	int atoi_result;
	struct sockaddr_in server;

	printf("\nTamanho do Pacote : %d Bytes\n", tam_data);

	for (i = 0; i < 18; ++i)
	{
		if (ligacao->enlaces[i][0] == ligacao->num_no && env_no == ligacao->enlaces[i][1])
		{
			if(tam_data > ligacao->enlaces[i][2])
				//return ligacao->enlaces[i][2];

			for (i = 0; i < 6; ++i)
			{
				atoi_result = atoi(ligacao->nos[i][0]);

				if (atoi_result == env_no)
				{
					if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
					perror("socket()");
					exit(1);
					}

					server.sin_family = AF_INET; /* Tipo do endereço         */
				    server.sin_port = htons(atoi(ligacao->nos[i][2])); /* Porta do servidor        */
				    server.sin_addr.s_addr = inet_addr(ligacao->nos[i][1]); /* Endereço IP do servidor  */
				}
			}

            if (sendto(s, &data, sizeof (data), 0, (struct sockaddr *) &server, sizeof (server)) < 0) {
                perror("sendto()");
                exit(2);
            }

			#ifdef DEBBUG
			printf("\nDados enviados: '%s'\n\n",(char *)data);
			#endif

			//return 1;
		}
	}
	//return -1;
}

int receberPacotes(void){

}

void colocarArquivoStruct(FILE * fp, int lendo){

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
				strcpy(ligacao.nos[i][j],pch);
				#ifdef DEBBUG
				printf("nos[%d][%d] '%s' | ",i,j,ligacao.nos[i][j]);
				#endif
				troca_i++;
	    	}else if (lendo = ENLACES)
	    	{
				ligacao.enlaces[i][j] = atoi(pch);
				#ifdef DEBBUG
				printf("enlace[%d][%d] '%d' | ",i,j,ligacao.enlaces[i][j]);
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