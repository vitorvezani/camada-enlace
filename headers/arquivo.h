//
//  arquivo.h
//
//  Created by Vitor Vezani on 26/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>		    /* for printf() */
#include <stdlib.h>			/* for exit() */
#include <string.h>
#include <pthread.h>     	/* para poder manipular threads */

#define DEBBUG

#define TRUE 			1
#define FALSE 			0

#define MAXNOS			6

#define NOS 			1
#define ENLACES 		2

struct ligacoes {
    char nos[MAXNOS][3][25];
    int enlaces[18][3];
};

void delete_espace(char* input);