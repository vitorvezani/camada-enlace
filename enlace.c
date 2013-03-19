//
//  enlace.c
//
//  Created by Vitor Vezani on 13/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

struct package{
    char nos[6][3];
    int enlaces[18][3];
};

void lerArquivo(char * nome_arq){

 	int result;
 	long lSize;
 	char * buffer;
	FILE * fp;
	fp = fopen(nome_arq, "r");

    if(!fp)
        perror("Fopen()");

	// obtain file size:
	fseek (fp , 0 , SEEK_END);
	lSize = ftell (fp);
	rewind (fp);

	// allocate memory to contain the whole file:
	buffer = (char*) malloc (sizeof(char)*lSize);
		if (buffer == NULL){
			fputs ("Memory error",stderr); 
			exit (2);
		}

    result = fread(buffer, 1, lSize, fp);
	    if (result != lSize){
	    	fputs ("Reading error",stderr);
	    	exit (3);
	    }

	colocarArquivoStruct(buffer);

	fclose (fp);
  	free (buffer);

}

void colocarArquivoStruct(char * buffer){
}
