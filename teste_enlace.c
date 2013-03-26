//
//  teste_enlace.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[])
{
	int num_no; 
	char nome_arq[20];
	int ret_enviar;
	char *data = "vitorasdasasdadasdasdasd";

	if (argc != 3) {
        printf("Use: %s 'nome_arq_config' 'numero_nó'\n", argv[0]);
        exit(1);
    }

    strcpy(nome_arq,argv[1]);
	num_no = atoi(argv[2]);

	printf("nome do arquivo: '%s'\n num do nó: '%d'\n",nome_arq,num_no);

	iniciarEnlace(nome_arq,num_no);

	ret_enviar = enviarPacote(strlen(data),2,data);
	
	printf("Return : ");

	if(ret_enviar == -1) printf("Enlace nao encontrado\n\n");
	else if (ret_enviar == 1) printf("OK\n\n");
	else printf("MTU Maximo : %d\n\n",ret_enviar);

	return 0;
}