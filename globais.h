//
//  header.h
//
//  Created by Vitor Vezani on 26/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>		    /* for printf() */
#include <stdlib.h>			/* for exit() */
#include <string.h>
#include <pthread.h>     	/* para poder manipular threads */

struct shm_rede_enlace{
	int type;
	int tam_buffer;
	int env_no;
	char buffer[2500];
	int erro;
};

struct shd_file_info{
	char nome_arq[20];
	int num_no;
};