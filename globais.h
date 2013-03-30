//
//  header.h
//
//  Created by Vitor Vezani on 26/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <sys/time.h>		/* for gettimeofday() */
#include <stdio.h>		    /* for printf() */
#include <unistd.h>		    /* for fork() */
#include <sys/types.h>		/* for wait(), msgget(), msgctl() */
#include <sys/wait.h>		/* for wait() */
#include <sys/ipc.h>		/* for msgget(), msgctl() */
#include <sys/msg.h>		/* for msgget(), msgctl() */
#include <errno.h>          /* errno and error codes */
#include <signal.h>         /* for kill(), sigsuspend(), others */
#include <sys/shm.h>        /* for shmget(), shmat(), shmctl() */
#include <sys/sem.h>        /* for semget(), semop(), semctl() */
#include <stdlib.h>			/* for exit() */
#include <string.h>
#include <pthread.h>     	/* para poder manipular threads */

typedef struct{
	int tam_buffer;
	int env_no;
	char buffer[2500];
	int erro;
}shm_rede_enlace;

pthread_mutex_t exc_aces;

struct shm_rede_enlace shm_ren;