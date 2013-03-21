//
//  enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG
#define NOS 			1
#define ENLACES 		2

void colocarArquivoStruct(FILE * fp, int lendo);
void retirarEspaco(char * string);

struct ligacoes{
    char nos[6][3][25];
    int enlaces[18][3];
} ligacao;

void abrirArquivo(char * nome_arq){

 	int result;
 	int lendo = 0;
 	int i,j;

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
	    	printf("\nTabela de nós\n");
		    lendo = NOS;
		    i=0;
	    }

	    if (strcmp(pch,"[Enlaces]") == 0)
	    {
	    	printf("\nTabela de enlaces\n");
		    lendo = ENLACES;
		    i=0;
	    }

	    if (strcmp(pch,"[Nos]") != 0 && strcmp(pch,"[Enlaces]") != 0)
	    {
	    	if (lendo == NOS)
	    	{
    			strcpy(ligacao.nos[i][j],pch);
    			#ifdef DEBUG
    			printf("nos[%d][%d] '%s' | ",i,j,ligacao.nos[i][j]);
    			#endif
    			troca_i++;
	    	}else if (lendo = ENLACES)
	    	{
    			ligacao.enlaces[i][j] = atoi(pch);
    			#ifdef DEBUG
    			printf("enlace[%d][%d] '%d' | ",i,j,ligacao.enlaces[i][j]);
	    		#endif
	    		troca_i++;
	    	}
			if(troca_i == 3){
				i++;
				printf("\n");
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