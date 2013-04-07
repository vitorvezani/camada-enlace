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
    int j, i = 0;
    int troca_i;
    int lendo = 0;
<<<<<<< HEAD
    int numbers[MAXNOS];
    int maxenlaces[MAXNOS];

    for (i = 0; i < MAXNOS; ++i){ 
        numbers[i] = 0;
        maxenlaces[i] = 0;
    }

=======
>>>>>>> Limitado no MAX 6 nós

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

                        strcpy(ligacao->nos[i][j], pch);
#ifdef DEBBUG
                        printf("nos[%d][%d] '%s' | ", i, j, ligacao->nos[i][j]);

#endif
                        troca_i++;
                    } else if (lendo = ENLACES) {
<<<<<<< HEAD

                    ligacao->enlaces[i][j] = atoi(pch);

/*
                    //Logica para encontrar + de 3 enlaces para cada nó
                    if ( j == 0)
                    {
                        for (k = 0; k < MAXNOS*3; ++k) {

                        if (numbers[k] == ligacao->enlaces[i][j])
                        {
                            maxenlaces[k] += 1;
                            printf("\nNo '%d' tem '%d' enlaces\n",numbers[k], maxenlaces[k]);
                            if (maxenlaces[k] >= 3)
                            {
                                printf("\nNumero maximo de enlace do nó '%d' atingido!\n",numbers[k]);
                                exit(1);
                            }
                        break;
                        }
                    }
                    }
*/
=======
                        ligacao->enlaces[i][j] = atoi(pch);
>>>>>>> Limitado no MAX 6 nós
#ifdef DEBBUG
                        printf("enlace[%d][%d] '%d' | ", i, j, ligacao->enlaces[i][j]);
#endif
                        troca_i++;
                    }
                    if (troca_i == 3) {
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
