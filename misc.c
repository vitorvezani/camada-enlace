//
//  misc.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//FUNCOES AUXILIARES DA CAMADA DE ENLACE

void retirarEspaco(char * string){
	int j,i = 0;
	char *stringAux = "";

	if ((stringAux = (char *) malloc(sizeof(char) * strlen(string) + 1)) == NULL) {
        printf("unable to allocate memory \n");
        exit (4); 
    }

	while(string[i]!='\0')
	{
		if(string[i]!=' '){ 
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