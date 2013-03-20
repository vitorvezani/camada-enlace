//
//  enlace.c
//
//  Created by Vitor Vezani on 13/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void colocarArquivoStruct(FILE * fp);
void retirarEspaco(char * string);

struct ligacoes{
    char nos[6][3];
    int enlaces[18][3];
} ligacao;

void abrirArquivo(char * nome_arq){

 	int result;
	FILE * fp;
	fp = fopen(nome_arq, "r");

    if(!fp)
        perror("Fopen()");

	colocarArquivoStruct(fp);

	fclose (fp);

}

void colocarArquivoStruct(FILE * fp){

size_t len= 100; // valor arbitrário
char *linha= malloc(len);
char * pch;

  while (getline(&linha, &len, fp) > 0)
  {

  pch = strtok (linha,">,:");

  	if (strlen(linha) != 1){

  	printf("\n");	
  	  while (pch != NULL)
	  {
	    retirarEspaco(pch);

	    if (strcmp(pch,"[Nos]") != 0 && strcmp(pch,"[Enlaces]") != 0)
	    {
	    	printf ("Tokenizer : '%s'\n",pch);
	    }

	    pch = strtok (NULL, ">,:");
	  }
	}
  }


  if(linha)
  	free(linha);
  fclose(fp);

}

void retirarEspaco(char * string){
	int j,i = 0;
	char *stringAux = "";

	if ((stringAux = (char *) malloc(sizeof(char) * strlen(string) + 1)) == NULL) {
        printf("unable to allocate memory \n");
        exit (4); 
    }

	while(string[i]!='\0') //aqui ele vai percorrer a string q vc digitou
	{
		if(string[i]!=' '){ //variável retirando-se os espaços em branco.....
			stringAux[j]=string[i];
			j++;
		}
		i++;
	}
		stringAux[j]='\0';

	    if (stringAux[strlen(stringAux)-1]=='\n')
	    {
	    	stringAux[strlen(stringAux)-1]='\0';
	    }

		strcpy(string,stringAux);

		free(stringAux);
}