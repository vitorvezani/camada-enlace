//
//  misc.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/misc.h"

//FUNCOES AUXILIARES DA CAMADA DE ENLACE

void delete_espace(char* input)                                         
{
	int i,j;
	size_t len = 100;

	char *output=malloc(len);
	strcpy(output,input);

    for (i = 0, j = 0; i<strlen(input); i++,j++)          
    {
        if (input[i]!=' ')                           
            output[j]=input[i];                     
        else
            j--;                                     
    }
    output[j]='\0';

	    if (output[strlen(output)-1]=='\n')
	    	output[strlen(output)-1]='\0';

    strcpy(input,output);
}
