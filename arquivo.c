//
//  misc.c
//
//  Created by Vitor Vezani on 19/03/13.
//  Copyright (c) 2013 Vitor Vezani. All rights reserved.
//

#include "headers/arquivo.h"

//FUNCOES DE ARQUIVO DA CAMADA DE ENLACE

void colocarArquivoStruct(FILE * fp, struct ligacoes *ligacao) {

    size_t len = 100;
    char *linha = malloc(len);
    char *pch;
    int j,i,k = 0;
    int troca_i;
    int lendo = 0;
    int numbers[MAXNOS];
    int maxenlaces[MAXNOS];

    for (i = 0; i < MAXNOS; ++i)
        maxenlaces[i] = 0;

    while (getline(&linha, &len, fp) > 0) {
        j = 0;
        troca_i = 0;

        pch = strtok(linha, ">,:");

        if (strlen(linha) != 1) { //enter somente com o '\n'

            while (pch != NULL) {
                //Deleta Espacos
                delete_espace(pch);

                if (strcmp(pch, "[Nos]") == 0) {
#ifdef DEBBUG
                    printf("\nTabela de nós\n");
#endif
                    lendo = NOS;
                    i = 0;
                }

                if (strcmp(pch, "[Enlaces]") == 0) {
#ifdef DEBBUG
                    printf("\nTabela de enlaces\n");
#endif
                    lendo = ENLACES;
                    i = 0;
                }

                if (strcmp(pch, "[Nos]") != 0 && strcmp(pch, "[Enlaces]") != 0) {
                    if (lendo == NOS) {
                        if (i >= 6)
                        {
                            printf("Limite de 6 nós atingidos\n");
                            exit(1);
                        }
                        if (j = 0)
                        {
                            numbers[i] = atoi(pch);
                        }

                        strcpy(ligacao->nos[i][j], pch);
#ifdef DEBBUG
                        printf("nos[%d][%d] '%s' | ", i, j, ligacao->nos[i][j]);
#endif
                        troca_i++;
                    } else if (lendo = ENLACES) {

/*
                    //Logica para encontrar + de 3 enlaces para cada nó
                    for (k = 0; k < MAXNOS*3; k++) {

                        if (numbers[k] == ligacao->enlaces[i][0])
                        {
                            maxenlaces[k] += 1;
                            //printf("No '%d' tem '%d' enlaces\n",numbers[k], maxenlaces[k]);
                            if (maxenlaces[k] >= 3)
                            {
                                printf("Numero maximo de enlace do nó '%d' atingido!\n",numbers[k]+1);
                                exit(1);
                            }
                        break;
                        }
                    }
*/
                        ligacao->enlaces[i][j] = atoi(pch);
#ifdef DEBBUG
                        printf("enlace[%d][%d] '%d' | ", i, j, ligacao->enlaces[i][j]);
#endif
                        troca_i++;
                    }
                    if (troca_i == 3){
                        i++;
#ifdef DEBBUG
                        printf("\n");
#endif
                    }
                }
                j++;
                pch = strtok(NULL, ">,:");
            }
        }
    }

    if (linha)
        free(linha);

    fclose(fp);
}

void delete_espace(char* input) {
    int i, j;
    size_t len = 100;

    char *output = malloc(len);
    strcpy(output, input);

    for (i = 0, j = 0; i < strlen(input); i++, j++) {
        if (input[i] != ' ')
            output[j] = input[i];
        else
            j--;
    }
    output[j] = '\0';

    if (output[strlen(output) - 1] == '\n')
        output[strlen(output) - 1] = '\0';

    strcpy(input, output);
}
